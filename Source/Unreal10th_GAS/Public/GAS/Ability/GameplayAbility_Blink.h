// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Blink.generated.h"

UCLASS()
class UNREAL10TH_GAS_API UGameplayAbility_Blink : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGameplayAbility_Blink();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual bool CheckCost(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
    virtual FVector CalculateBlinkDestination(const ACharacter* InCharacter, float InDistance) const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Blink")
    void OnBlinkExecuted(const FVector& StartLocation, const FVector& DestinationLocation);

protected:
    // 레벨별 이동 거리 (테이블 적용 가능)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blink|Distance")
    FScalableFloat BlinkDistance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blink|Collision")
    float CollisionOffset = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blink|Collision")
    float StepOffset = 25.0f;

    // 이동 후 캐릭터를 바닥에 착지 시킬지 여부
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blink|Collision")
    bool bTraceFloor = true;

    // 바닥 감지 최대 깊이
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blink|Collision")
    float TraceFloorDistance = 500.0f;

};
