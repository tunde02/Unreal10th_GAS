// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_SuperJump.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

UGameplayAbility_SuperJump::UGameplayAbility_SuperJump()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_SuperJump::ActivateAbility(
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

    // AbilitySystemComponent가 없으면 종료
    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (!ASC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 점프력 게이지 증가 이펙트 적용
    if (SuperJumpGaugeEffectClass)
    {
        FGameplayEffectSpecHandle GaugeSpecHandle = MakeOutgoingGameplayEffectSpec(
            Handle, ActorInfo, ActivationInfo, SuperJumpGaugeEffectClass, GetAbilityLevel(Handle, ActorInfo));

        if (GaugeSpecHandle.IsValid())
        {
            GaugeEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, GaugeSpecHandle);
        }
    }

    UAbilityTask_WaitInputRelease* WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
    if (WaitReleaseTask)
    {
        WaitReleaseTask->OnRelease.AddDynamic(this, &UGameplayAbility_SuperJump::OnWaitInputReleaseCallback);
        WaitReleaseTask->ReadyForActivation();
    }
}

void UGameplayAbility_SuperJump::EndAbility(
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

    // 이펙트 제거
    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (GaugeEffectHandle.IsValid())
    {
        ASC->RemoveActiveGameplayEffect(GaugeEffectHandle); // 이펙트 제거하고
        GaugeEffectHandle.Invalidate(); // 활성 이펙트 핸들 초기화
    }

    ASC->ApplyModToAttributeUnsafe(UStatAttributeSet::GetJumpGaugeAttribute(), EGameplayModOp::Override, 0.0f);

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_SuperJump::OnWaitInputPressCallback(float InElapsedTime)
{
}

void UGameplayAbility_SuperJump::OnWaitInputReleaseCallback(float InTimeHeld)
{
    if (IsActive())
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}
