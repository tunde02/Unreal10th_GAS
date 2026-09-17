// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/MMC/MMC_Damage.h"
#include "GAS/StatAttributeSet.h"

#include "GameplayEffectExecutionCalculation.h"

//==================================================================================
// Custom Calculation 관련 어트리뷰트 캡쳐를 위한 정형화된 패턴

struct FDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);

    FDamageStatics()
    {
        // 소스의 공격력을 생성 시점 기준으로 캡쳐한다
        DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, AttackPower, Source, true);

        // 소스의 방어력을 피격 시점 기준으로 캡쳐한다
        DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, DefensePower, Target, false);
    }
};

static const FDamageStatics& DamageStatics()
{
    static FDamageStatics DStatics;
    return DStatics;
}

UMMC_Damage::UMMC_Damage()
{
    // 어떤 어트리뷰트를 캡쳐할 것인지 지정
    RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().DefensePowerDef);
}

//==================================================================================

float UMMC_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    FAggregatorEvaluateParameters EvalParams;

    float Attack = 0.0f;
    GetCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, Spec, EvalParams, Attack);

    float Defense = 0.0f;
    GetCapturedAttributeMagnitude(DamageStatics().DefensePowerDef, Spec, EvalParams, Defense);

    float Damage = FMath::Max(1.0f, Attack - Defense);

    return Damage;
}
