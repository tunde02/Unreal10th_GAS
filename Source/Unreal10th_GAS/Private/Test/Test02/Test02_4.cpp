// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Test02/Test02_4.h"
#include "Test/TestCharacter.h"

#include "AbilitySystemComponent.h"

void ATest02_4::ApplyGameplayEffect()
{
    if (!Source) return;
    if (!Target) return;
    if (!GameplayEffectClass) return;

    UAbilitySystemComponent* SourceASC = Source->GetAbilitySystemComponent();
    if (!SourceASC) return;

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) return;

    FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
    EffectContext.AddSourceObject(Source);
    EffectContext.AddInstigator(Source, Source);

    FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContext);
    if (!SpecHandle.IsValid()) return;

    FActiveGameplayEffectHandle ActiveEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

void ATest02_4::AddTag()
{
    if (!Target) { return; }

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) { return; }

    TargetASC->AddLooseGameplayTag(TagToAdd);
}

void ATest02_4::RemoveAllTag()
{
    if (!Target) { return; }

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) { return; }

    TargetASC->RemoveLooseGameplayTag(TagToAdd);
}
