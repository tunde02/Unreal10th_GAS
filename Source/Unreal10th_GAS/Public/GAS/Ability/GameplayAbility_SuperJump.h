// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_SuperJump.generated.h"

UCLASS()
class UNREAL10TH_GAS_API UGameplayAbility_SuperJump : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGameplayAbility_SuperJump();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

private:
    // AbilityTask_WaitInputPress용 콜백
    UFUNCTION()
    void OnWaitInputPressCallback(float InElapsedTime);

    // AbilityTask_WaitInputRelease용 콜백
    UFUNCTION()
    void OnWaitInputReleaseCallback(float InTimeHeld);

protected:
    // 점프 게이지 증가 이펙트 (Infinite)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SuperJump|Effects")
    TSubclassOf<UGameplayEffect> SuperJumpGaugeEffectClass;

private:
    FActiveGameplayEffectHandle GaugeEffectHandle;

};
