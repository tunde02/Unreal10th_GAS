// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/StatAttributeSet.h"

UStatAttributeSet::UStatAttributeSet()
{
    InitHealth(100.0f);
    InitMaxHealth(100.0f);

    InitStamina(100.0f);
    InitMaxStamina(100.0f);
}

void UStatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    else if (Attribute == GetMaxHealthAttribute())
    {
        NewValue = FMath::Max(0.0f, NewValue);
    }
    else if (Attribute == GetStaminaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
    }
    else if (Attribute == GetDefenseAttribute())
    {
        NewValue = FMath::Max(0.0f, NewValue);
    }
}

void UStatAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        UE_LOG(LogTemp, Log, TEXT("[UStatAttributeSet::PostAttributeChange()] : Health 변경됨 (%.1f) -> (%.1f)"),
               OldValue, NewValue);
    }
    else if (Attribute == GetMaxHealthAttribute())
    {
        // 최대 체력이 증가하면 그 수치만큼 현재 체력도 증가한다
        // 최대 체력이 감소하면 최대 체력을 초과하는 만큼만 제거한다

        const float CurrentHealth = GetHealth();
        float NewHealth = CurrentHealth;

        if (NewValue > OldValue)
        {
            NewHealth += NewValue - OldValue;
        }

        NewHealth = FMath::Clamp(NewHealth, 0.0f, NewValue);

        SetHealth(NewHealth);
    }
    else if (Attribute == GetStaminaAttribute())
    {
        UE_LOG(LogTemp, Log, TEXT("[UStatAttributeSet::PostAttributeChange()] : Stamina 변경됨 (%.1f) -> (%.1f)"),
               OldValue, NewValue);
    }
}

void UStatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        const float LocalDamage = GetDamage();
        SetDamage(0.0f);

        if (LocalDamage > 0)
        {
            // 여기서 데미지에 대한 각종 계산 추가
            // ex) 방어력, 최소 대미지, 쉴드 피해 증가 등
            const float FinalDamage = FMath::Clamp(LocalDamage - GetDefense(), 0.0f, GetMaxHealth());
            const float NewHealth = FMath::Clamp(GetHealth() - FinalDamage, 0.0f, GetMaxHealth());
            SetHealth(NewHealth);
        }
    }
    else if (Data.EvaluatedData.Attribute == GetStaminaCostAttribute())
    {
        const float LocalStaminaCost = GetStaminaCost();
        SetStaminaCost(0.0f);

        if (LocalStaminaCost > 0)
        {
            const float FinalStaminaCost = LocalStaminaCost;
            const float NewStamina = FMath::Clamp(GetStamina() - FinalStaminaCost, 0.0f, GetMaxStamina());
            SetStamina(NewStamina);
        }
    }
}
