// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EC_StackDamage.generated.h"

UCLASS()
class UNREAL10TH_GAS_API UEC_StackDamage : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UEC_StackDamage();

    virtual void Execute_Implementation(
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TSubclassOf<UGameplayEffect> DebuffEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float BaseDamage = 1.0f;

};
