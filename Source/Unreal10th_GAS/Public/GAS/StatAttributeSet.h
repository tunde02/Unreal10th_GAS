// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "StatAttributeSet.generated.h"

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
    // 값의 변화 감지나, UI 반영을 위해 사용
    virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

    // 이펙트가 적용된 후에 실행되는 함수
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
    // 최대값이 변경되었을 때 현재 값을 보정하는 함수
    void AdjustAttributeForMaxChange(
        float InOldMaxValue,
        float InNewMaxValue,
        const FGameplayAttribute& InAffectedAttributeProperty);

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

    UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, AttackPower);

    UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
    FGameplayAttributeData DefensePower;
    ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, DefensePower);

    UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, Damage);

    UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
    FGameplayAttributeData StaminaCost;
    ATTRIBUTE_ACCESSORS_BASIC(UStatAttributeSet, StaminaCost);

};
