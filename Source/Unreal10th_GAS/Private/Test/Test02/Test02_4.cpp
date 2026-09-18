// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Test02/Test02_4.h"
#include "Test/TestCharacter.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"

void ATest02_4::ApplyGameplayEffect()
{
    if (!Target) { return; }
    if (!Source) { return; }
    if (!GameplayEffectClass) { return; }

    UAbilitySystemComponent* SourceASC = Source->GetAbilitySystemComponent();
    if (!SourceASC) { return; }

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) { return; }

    FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);
    EffectContext.AddInstigator(Source, Source);

    FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContext);
    if (!SpecHandle.IsValid()) { return; }

    FActiveGameplayEffectHandle ActiveEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

void ATest02_4::ApplyCurseEffect()
{
    if (!Target) { return; }
    if (!GameplayEffectClass) { return; }

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) { return; }

    FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
    EffectContext.AddSourceObject(Target);
    EffectContext.AddInstigator(Target, Target);

    FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(CurseEffectClass, 1.0f, EffectContext);

    FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ATest02_4::ApplyUndeadEffect()
{
    if (!Target) { return; }
    if (!GameplayEffectClass) { return; }

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) { return; }

    FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
    EffectContext.AddSourceObject(Target);
    EffectContext.AddInstigator(Target, Target);

    FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(UndeadEffectClass, 1.0f, EffectContext);

    FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ATest02_4::SetHealthToMax()
{
    if (!Target) { return; }

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) { return; }

    TargetASC->ApplyModToAttributeUnsafe(UStatAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, 100.0f);
}
