// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_ChargingJump.generated.h"

UCLASS()
class UNREAL10TH_GAS_API UGameplayAbility_ChargingJump : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGameplayAbility_ChargingJump();

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
    UFUNCTION()
    void OnWaitInputReleaseCallback(float TimeHeld);

    UFUNCTION()
    void OnMovementModeChangedCallback(EMovementMode NewMovementMode);

    // 실제 점프 처리용 함수
    void ExecuteChargeJump(float InTimeHeld);

    // 차지 진행 처리용 타이머 콜백 (UI용)
    //void UpdateChargeProgress();

    // 각종 리소스 정리용 함수
    void CleanupChargeState();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Physics", meta = (ClampMin = "100.0"))
    float MinJumpVelocity = 420.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Physics", meta = (ClampMin = "100.0"))
    float MaxJumpVelocity = 1200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Charge", meta = (ClampMin = "0.0"))
    float MinHoldTime = 0.05f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Charge", meta = (ClampMin = "0.1"))
    float MaxHoldTime = 1.0f;

    // 최대 충전 시간에 도달하면 자동으로 점프 할지 여부
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Charge")
    bool bAutoReleaseOnMaxHold = true;

    // 차징 중 이동속도 감소 이펙트
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Effects")
    TSubclassOf<UGameplayEffect> ChargeSlowEffectClass;

    // 차징 중 스태미나 감소 이펙트
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Effects")
    TSubclassOf<UGameplayEffect> ChargeCostEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChargingJump|Cost", meta = (ClampMin = "0.0"))
    float MinStaminaToActivate = 10.0f;

private:
    FActiveGameplayEffectHandle ChargeSlowEffectHandle;
    FActiveGameplayEffectHandle ChargeCostEffectHandle;

    float ChargeStartTime = 0.0f;

    // 차지 진행용 타이머 핸들 (UI용)
    //FTimerHandle ChargeProgressTimerHandle;

    // 최대 차지시 자동 점프 타이머 핸들
    FTimerHandle AutoReleaseTimerHandle;

    // 현재 점프를 한 상태인지 여부
    bool bJumpExecuted = false;

};
