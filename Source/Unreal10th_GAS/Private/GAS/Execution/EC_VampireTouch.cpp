// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Execution/EC_VampireTouch.h"
#include "GAS/StatAttributeSet.h"

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
    TargetWeakTag = FGameplayTag::RequestGameplayTag(FName("GAS.Test.Debuff.Curse"), false);
    SourceAdvantageTag = FGameplayTag::RequestGameplayTag(FName("GAS.Test.Type.Undead"), false);
}

void UEC_VampireTouch::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvalParams;
    EvalParams.SourceTags = SourceTags;
    EvalParams.TargetTags = TargetTags;

    // 공격력 가져오기
    float SourceAttack = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        VampireTouchStatics().AttackPowerDef, EvalParams, SourceAttack);

    // 가져온 공격력을 기반으로 1차 대미지 계산
    float FinalDamage = BaseDamage * SourceAttack;

    // 타겟에게 특정 태그가 있으면 대미지 2배
    if (TargetTags && TargetTags->HasTag(TargetWeakTag))
    {
        FinalDamage *= 2.0f;
    }

    if (FinalDamage > 0.0f)
    {
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(
                UStatAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, FinalDamage));

        UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
        if (SourceTags && SourceTags->HasTag(SourceAdvantageTag))
        {
            SourceASC->ApplyModToAttributeUnsafe(
                UStatAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, FinalDamage * HealRate);
        }
        else
        {
            SourceASC->ApplyModToAttributeUnsafe(
                UStatAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, FinalDamage * RecoilRate);
        }
    }
}
