// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Execution/EC_VampireTouch.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"

struct FVampireTouchStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);

    FVampireTouchStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, AttackPower, Source, true);
    }
};

static const FVampireTouchStatics& VampireTouchStatics()
{
    static FVampireTouchStatics DStatics;
    return DStatics;
}

UEC_VampireTouch::UEC_VampireTouch()
{
    RelevantAttributesToCapture.Add(VampireTouchStatics().AttackPowerDef);
}

void UEC_VampireTouch::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    if (!SourceASC) { return; }

    FAggregatorEvaluateParameters EvalParams;

    // 대미지 계산
    float AttackPower = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(VampireTouchStatics().AttackPowerDef, EvalParams, AttackPower);

    float FinalDamage = BaseDamage + AttackPower;
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    // 타겟이 Curse 태그를 가지고 있으면 대미지 2배
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    if (TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("GAS.Test.Debuff.Curse"))))
    {
        FinalDamage *= CurseMultiplier;
    }

    // 대미지 모디파이어 추가
    if (FinalDamage > 0.0f)
    {
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(UStatAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, FinalDamage)
        );
    }

    // 시전자 회복 여부 결정
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    if (SourceTags->HasTag(FGameplayTag::RequestGameplayTag(FName("GAS.Test.Type.Undead"))))
    {
        // 시전자가 Undead 태그를 가지고 있으면 체력 회복
        const float HealAmount = FinalDamage * HealthGainPercent;
        SourceASC->ApplyModToAttributeUnsafe(UStatAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, HealAmount);
    }
    else
    {
        // 시전자가 Undead 태그를 가지고 있지 않으면 체력 소모
        const float HealthCostAmount = FinalDamage * HealthCostPercent;
        SourceASC->ApplyModToAttributeUnsafe(UStatAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, HealthCostAmount);
    }
}
