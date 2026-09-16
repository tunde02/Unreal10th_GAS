// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Test02/Test02_1.h"
#include "Test/TestCharacter.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"

void ATest02_1::ApplyGameplayEffect()
{
	if (!Target)
	{
		return;
	}

	if (!GameplayEffectClass)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	EffectContext.AddInstigator(GetInstigator(), this);

	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContext);

	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	if (ActiveEffectHandle.WasSuccessfullyApplied())
	{
		// 성공
	}
	else
	{
		// 실패
	}

	// Instant가 아닌 이펙트 중 핸들로 관리하던 이펙트를 제거
	//TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
}

void ATest02_1::RemoveAllEffects()
{
	if (!Target)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}

	FGameplayEffectQuery Query; // 쿼리를 생성하기만 하면 기본적으로 모든 이펙트에 해당함
	TargetASC->RemoveActiveEffects(Query);
}

void ATest02_1::ResetTarget()
{
	if (!Target)
	{
		return;
	}

	RemoveAllEffects();
	UStatAttributeSet* Stat = Target->GetStatAttribute();
	Stat->SetHealth(Stat->GetMaxHealth());
}
