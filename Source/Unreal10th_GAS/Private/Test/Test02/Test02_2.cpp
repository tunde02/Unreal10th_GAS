// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Test02/Test02_2.h"
#include "GAS/StatAttributeSet.h"
#include "Test/TestCharacter.h"

#include "AbilitySystemComponent.h"

ATest02_2::ATest02_2()
{
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    Stat = CreateDefaultSubobject<UStatAttributeSet>(TEXT("StatAttributeSet"));
}

void ATest02_2::ApplyGameplayEffect()
{
    if (!Target) return;
    if (!GameplayEffectClass) return;
    if (!ASC) return;
    if (!Stat) return;

    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!TargetASC) return;

    FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);
    EffectContext.AddInstigator(this, this);

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContext);
    if (!SpecHandle.IsValid()) return;

    FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

UAbilitySystemComponent* ATest02_2::GetAbilitySystemComponent() const
{
    return ASC;
}

UStatAttributeSet* ATest02_2::GetStatAttributeSet() const
{
    return Stat;
}

void ATest02_2::BeginPlay()
{
    Super::BeginPlay();

    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
    }
}
