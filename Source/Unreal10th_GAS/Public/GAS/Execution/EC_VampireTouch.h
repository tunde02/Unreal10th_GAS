// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EC_VampireTouch.generated.h"

UCLASS()
class UNREAL10TH_GAS_API UEC_VampireTouch : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UEC_VampireTouch();

    virtual void Execute_Implementation(
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float BaseDamage = 3.0f;

private:
    const float CurseMultiplier = 2.0f;
    const float HealthCostPercent = -0.1f;
    const float HealthGainPercent = 0.2f;

};
