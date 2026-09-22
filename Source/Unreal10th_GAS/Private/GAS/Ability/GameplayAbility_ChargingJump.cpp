// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_ChargingJump.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

UGameplayAbility_ChargingJump::UGameplayAbility_ChargingJump()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    FGameplayTag GroundedTag = FGameplayTag::RequestGameplayTag(FName("GAS.State.Grounded"), false);
    if (GroundedTag.IsValid())
    {
        // 해당 태그가 있어야만 이 어빌리티가 발동되도록 설정
        ActivationRequiredTags.AddTag(GroundedTag);
    }

    FGameplayTag JumpingTag = FGameplayTag::RequestGameplayTag(FName("GAS.State.Jumping"), false);
    if (JumpingTag.IsValid())
    {
        // 이 어빌리티가 발동되면 자동으로 해당 태그를 부여하도록 설정
        ActivationOwnedTags.AddTag(JumpingTag);

        // 이 태그가 있으면 어빌리티 발동 불가능하도록 설정
        ActivationBlockedTags.AddTag(JumpingTag);
    }
}

void UGameplayAbility_ChargingJump::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    bJumpExecuted = false;
    ChargeStartTime = GetWorld()->GetTimeSeconds();

    if (ChargeCostEffectClass)
    {
        FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(
            Handle, ActorInfo, ActivationInfo, ChargeCostEffectClass, GetAbilityLevel(Handle, ActorInfo));

        if (CostSpecHandle.IsValid())
        {
            ChargeCostEffectHandle = ApplyGameplayEffectSpecToOwner(
                Handle, ActorInfo, ActivationInfo, CostSpecHandle);
        }
    }

    if (ChargeSlowEffectClass)
    {
        FGameplayEffectSpecHandle SlowSpecHandle = MakeOutgoingGameplayEffectSpec(
            Handle, ActorInfo, ActivationInfo, ChargeSlowEffectClass, GetAbilityLevel(Handle, ActorInfo));

        if (SlowSpecHandle.IsValid())
        {
            ChargeSlowEffectHandle = ApplyGameplayEffectSpecToOwner(
                Handle, ActorInfo, ActivationInfo, SlowSpecHandle);
        }
    }

    // UI용 타이머
    //FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    //TimerManager.SetTimer(
    //    /* InOutHandle   */ ChargeProgressTimerHandle,
    //    /* InObj         */ this,
    //    /* InTimerMethod */ &UGameplayAbility_ChargingJump::UpdateChargeProgress,
    //    /* InRate        */ 0.016f,
    //    /* InbLoop       */ true);

    if (bAutoReleaseOnMaxHold && MaxHoldTime > 0.0f)
    {
        FTimerManager& TimerManager = GetWorld()->GetTimerManager();
        FTimerDelegate AutoReleaseDelegate;

        // FTimerDelegate는 void() 형식만 바인드 가능한데
        // 이렇게 하면 void ExecuteChargeJump(MaxHoldTime) 을 바인드한 것과 같아진다
        AutoReleaseDelegate.BindUObject(this, &UGameplayAbility_ChargingJump::ExecuteChargeJump, MaxHoldTime);

        TimerManager.SetTimer(
            /* InOutHandle */ AutoReleaseTimerHandle,
            /* InDelegate  */ AutoReleaseDelegate,
            /* InRate      */ MaxHoldTime,
            /* InbLoop     */ false);
    }

    UAbilityTask_WaitInputRelease* WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
    if (WaitReleaseTask)
    {
        WaitReleaseTask->OnRelease.AddDynamic(this, &UGameplayAbility_ChargingJump::OnWaitInputReleaseCallback);
        WaitReleaseTask->ReadyForActivation();
    }
}

void UGameplayAbility_ChargingJump::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    if (!IsActive()) { return; }

    bJumpExecuted = false;

    CleanupChargeState();

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGameplayAbility_ChargingJump::CheckCost(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
    {
        return false;
    }

    const UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (ASC && MinStaminaToActivate > 0.0f)
    {
        const float CurrentStamina = ASC->GetNumericAttribute(UStatAttributeSet::GetStaminaAttribute());
        if (CurrentStamina < MinStaminaToActivate)
        {
            const FGameplayTag& CostTag = UAbilitySystemGlobals::Get().ActivateFailCostTag;
            if (OptionalRelevantTags && CostTag.IsValid())
            {
                OptionalRelevantTags->AddTag(CostTag);
            }

            return false;
        }
    }

    return true;
}

void UGameplayAbility_ChargingJump::OnWaitInputReleaseCallback(float TimeHeld)
{
    ExecuteChargeJump(TimeHeld);
}

void UGameplayAbility_ChargingJump::OnMovementModeChangedCallback(EMovementMode NewMovementMode)
{
    // 착지를 완료해야 어빌리티 정상 종료
    if (NewMovementMode == MOVE_Walking)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UGameplayAbility_ChargingJump::ExecuteChargeJump(float InTimeHeld)
{
    if (bJumpExecuted) { return; }

    bJumpExecuted = true;

    // 타이머, 이펙트 해제
    CleanupChargeState();

    // 충전율 계산 (0.0 ~ 1.0)
    const float EffectiveTime = FMath::Clamp(InTimeHeld, 0.0f, MaxHoldTime);
    float ChargeAlpha = 0.0f;
    if (MaxHoldTime > MinHoldTime)
    {
        ChargeAlpha = FMath::Clamp((EffectiveTime - MinHoldTime) / (MaxHoldTime - MinHoldTime), 0.0f, 1.0f);
    }

    // 최종 점프 속도 계산
    const float FinalZVelocity = FMath::Lerp(MinJumpVelocity, MaxJumpVelocity, ChargeAlpha);

    // 실제 점프 처리
    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()); // Avatar : 내가 실제로 조종하고 있는 캐릭터
    if (Character)
    {
        Character->LaunchCharacter(FVector(0.0f, 0.0f, FinalZVelocity), /* bXYOverride */ false, /* bZOverride */ true);
    }

    // 착지 감지 태스크 만들기
    UAbilityTask_WaitMovementModeChange* WaitLandingTask =
        UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, MOVE_Walking);
    if (WaitLandingTask)
    {
        WaitLandingTask->OnChange.AddDynamic(this, &UGameplayAbility_ChargingJump::OnMovementModeChangedCallback);
        WaitLandingTask->ReadyForActivation();
    }
}

/*
void UGameplayAbility_ChargingJump::UpdateChargeProgress()
{
    if (!IsActive() || bJumpExecuted) { return; }

    const float CurrentTime = GetWorld()->GetTimeSeconds();
    const float ElapsedTime = CurrentTime - ChargeStartTime;
    const float Ratio = (MaxHoldTime > 0.0f) ? FMath::Clamp(ElapsedTime / MaxHoldTime, 0.0f, 1.0f) : 1.0f;

    // UI 업데이트 처리
}
*/

void UGameplayAbility_ChargingJump::CleanupChargeState()
{
    // 타이머 정리
    if (UWorld* World = GetWorld())
    {
        FTimerManager& TimerManager = World->GetTimerManager();
        //TimerManager.ClearTimer(ChargeProgressTimerHandle);
        TimerManager.ClearTimer(AutoReleaseTimerHandle);
    }

    // 이펙트 해제
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        if (ChargeSlowEffectHandle.IsValid())
        {
            ASC->RemoveActiveGameplayEffect(ChargeSlowEffectHandle);
        }

        ChargeSlowEffectHandle.Invalidate();

        if (ChargeCostEffectHandle.IsValid())
        {
            ASC->RemoveActiveGameplayEffect(ChargeCostEffectHandle);
        }

        ChargeCostEffectHandle.Invalidate();
    }
}
