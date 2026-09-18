// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/MMC/MMC_Critical_False.h"
#include "GAS/StatAttributeSet.h"

#include "GameplayEffectExecutionCalculation.h"

struct FDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);

    FDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, AttackPower, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, CriticalChance, Source, true);
    }
};

static const FDamageStatics& DamageStatics()
{
    static FDamageStatics DStatics;
    return DStatics;
}

UMMC_Critical_False::UMMC_Critical_False()
{
    RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
}

float UMMC_Critical_False::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    FAggregatorEvaluateParameters EvalParams;

    float AttackPower = 0.0f;
    GetCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, Spec, EvalParams, AttackPower);

    float Damage = FMath::Max(1.0f, AttackPower);

    float CriticalChance = 0.0f;
    GetCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef, Spec, EvalParams, CriticalChance);

    if (CriticalChance > FMath::FRand())
    {
        Damage *= 2.0f;
    }

    return Damage;
}