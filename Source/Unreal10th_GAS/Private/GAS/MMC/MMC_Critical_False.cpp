// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/MMC/MMC_Critical_False.h"
#include "GAS/StatAttributeSet.h"

#include "GameplayEffectExecutionCalculation.h"

struct FDeprecatedDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);

    FDeprecatedDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, AttackPower, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, CriticalChance, Source, true);
    }
};

static const FDeprecatedDamageStatics& DeprecatedDamageStatics()
{
    static FDeprecatedDamageStatics DStatics;
    return DStatics;
}

UMMC_Critical_False::UMMC_Critical_False()
{
    RelevantAttributesToCapture.Add(DeprecatedDamageStatics().AttackPowerDef);
    RelevantAttributesToCapture.Add(DeprecatedDamageStatics().CriticalChanceDef);
}

float UMMC_Critical_False::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    FAggregatorEvaluateParameters EvalParams;

    float AttackPower = 0.0f;
    GetCapturedAttributeMagnitude(DeprecatedDamageStatics().AttackPowerDef, Spec, EvalParams, AttackPower);

    float Damage = FMath::Max(1.0f, AttackPower);

    float CriticalChance = 0.0f;
    GetCapturedAttributeMagnitude(DeprecatedDamageStatics().CriticalChanceDef, Spec, EvalParams, CriticalChance);

    if (CriticalChance > FMath::FRand())
    {
        Damage *= 2.0f;
    }

    return Damage;
}