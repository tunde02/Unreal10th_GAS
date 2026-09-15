// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "StatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL10TH_GAS_API UStatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UStatAttributeSet();

	// CurrentValue 변경 전에 실행되는 함수
	// 값의 Clamping 용도로 사용
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// CurrentValue 변경 후에 실행되는 함수
	// 
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, Stamina);
	
	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, MaxStamina);

};
