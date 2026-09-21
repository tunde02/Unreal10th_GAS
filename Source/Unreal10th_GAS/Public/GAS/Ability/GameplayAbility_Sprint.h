// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Sprint.generated.h"

UCLASS()
class UNREAL10TH_GAS_API UGameplayAbility_Sprint : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGameplayAbility_Sprint();

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

    virtual bool CheckCost(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

private:
    // 스태미나 변경 시 실행될 콜백 함수
    void OnStaminaChanged(const FOnAttributeChangeData& InData);

    // AbilityTask_WaitInputPress용 콜백
    UFUNCTION()
    void OnWaitInputPressCallback(float InElapsedTime);

    // AbilityTask_WaitInputRelease용 콜백
    UFUNCTION()
    void OnWaitInputReleaseCallback(float InTimeHeld);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint|Input")
    bool bToggleMode = false;

    // 이동속도 증가 이펙트 (Infinite)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint|Effects")
    TSubclassOf<UGameplayEffect> SprintBuffEffectClass;

    // 주기적으로 스태미나를 감소시키는 이펙트 (Periodic Infinite)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint|Effects")
    TSubclassOf<UGameplayEffect> SprintCostEffectClass;

    // 어빌리티 발동에 필요한 최소 스태미나 수치
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint|Cost")
    float MinStaminaToActivate = 5.0f;

private:
    FActiveGameplayEffectHandle BuffEffectHandle;
    FActiveGameplayEffectHandle CostEffectHandle;

    // 스태미나 변경 감시용 델리게이트
    FDelegateHandle StaminaChangeDelegateHandle;

};
