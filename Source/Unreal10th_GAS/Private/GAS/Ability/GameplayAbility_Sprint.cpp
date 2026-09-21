// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_Sprint.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

UGameplayAbility_Sprint::UGameplayAbility_Sprint()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Sprint::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    // 코스트와 쿨다운 검사 후, 가능하면 적용
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // AbilitySystemComponent가 없으면 종료
    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (!ASC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 이동속도 증가 버프 적용
    if (SprintBuffEffectClass)
    {
        FGameplayEffectSpecHandle BuffSpecHandle = MakeOutgoingGameplayEffectSpec(
            Handle, ActorInfo, ActivationInfo, SprintBuffEffectClass, GetAbilityLevel(Handle, ActorInfo));

        if (BuffSpecHandle.IsValid())
        {
            BuffEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, BuffSpecHandle);
        }
    }

    // 스태미나 소모 감소 이펙트 적용
    if (SprintCostEffectClass)
    {
        FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(
            Handle, ActorInfo, ActivationInfo, SprintCostEffectClass, GetAbilityLevel(Handle, ActorInfo));

        if (CostSpecHandle.IsValid())
        {
            CostEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CostSpecHandle);
        }
    }

    // 스태미나 변화 감시
    FOnGameplayAttributeValueChange& StaminaChangeDelegate = ASC->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetStaminaAttribute());
    StaminaChangeDelegateHandle = StaminaChangeDelegate.AddUObject(this, &UGameplayAbility_Sprint::OnStaminaChanged);

    // 입력 방식에 따라 Task 등록
    if (bToggleMode)
    {
        UAbilityTask_WaitInputPress* WaitPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
        if (WaitPressTask)
        {
            WaitPressTask->OnPress.AddDynamic(this, &UGameplayAbility_Sprint::OnWaitInputPressCallback);
            WaitPressTask->ReadyForActivation();
        }
    }
    else
    {
        UAbilityTask_WaitInputRelease* WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
        if (WaitReleaseTask)
        {
            WaitReleaseTask->OnRelease.AddDynamic(this, &UGameplayAbility_Sprint::OnWaitInputReleaseCallback);
            WaitReleaseTask->ReadyForActivation();
        }
    }
}

void UGameplayAbility_Sprint::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    if (!IsActive())
    {
        return;
    }

    // 스태미나 변화 감시 델리게이트 해제
    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (ASC && StaminaChangeDelegateHandle.IsValid())
    {
        FOnGameplayAttributeValueChange& StaminaChangeDelegate = ASC->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetStaminaAttribute());
        StaminaChangeDelegate.Remove(StaminaChangeDelegateHandle); // 델리게이트 연결 해제
        StaminaChangeDelegateHandle.Reset(); // 델리게이트 핸들 리셋
    }

    // 이펙트 제거
    if (BuffEffectHandle.IsValid())
    {
        ASC->RemoveActiveGameplayEffect(BuffEffectHandle); // 이펙트 제거하고
        BuffEffectHandle.Invalidate(); // 활성 이펙트 핸들 초기화
    }

    if (CostEffectHandle.IsValid())
    {
        ASC->RemoveActiveGameplayEffect(CostEffectHandle);
        CostEffectHandle.Invalidate();
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGameplayAbility_Sprint::CheckCost(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (!ASC) { return false; }

    if (!ASC->HasAttributeSetForAttribute(UStatAttributeSet::GetStaminaAttribute())
        || !ASC->HasAttributeSetForAttribute(UStatAttributeSet::GetStaminaCostAttribute()))
    {
        return false;
    }

    // 시전자의 현재 Stamina값 가져오기
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

    return true;
}

void UGameplayAbility_Sprint::OnStaminaChanged(const FOnAttributeChangeData& InData)
{
    // 이 어빌리티가 발동 중이고, 스태미나가 0 이하일 때 처리
    if (IsActive() && InData.NewValue <= 0.0f)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UGameplayAbility_Sprint::OnWaitInputPressCallback(float InElapsedTime)
{
    if (bToggleMode && IsActive())
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UGameplayAbility_Sprint::OnWaitInputReleaseCallback(float InTimeHeld)
{
    if (!bToggleMode && IsActive())
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}
