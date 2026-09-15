// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HealthAndStaminaWidget.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthAndStaminaWidget::BindToAttribute(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayAttribute InCurrentHealthAttribute, FGameplayAttribute InMaxHealthAttribute, FGameplayAttribute InCurrentStaminaAttribute, FGameplayAttribute InMaxStaminaAttribute)
{
    if (!InAbilitySystemComponent)
        return;

    AbilitySystemComponent = InAbilitySystemComponent;
    CurrentHealthAttribute = InCurrentHealthAttribute;
    MaxHealthAttribute = InMaxHealthAttribute;
    CurrentStaminaAttribute = InCurrentStaminaAttribute;
    MaxStaminaAttribute = InMaxStaminaAttribute;

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentHealthAttribute)
        .AddUObject(this, &UHealthAndStaminaWidget::OnAttributeChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxHealthAttribute)
        .AddUObject(this, &UHealthAndStaminaWidget::OnAttributeChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentStaminaAttribute)
        .AddUObject(this, &UHealthAndStaminaWidget::OnAttributeChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxStaminaAttribute)
        .AddUObject(this, &UHealthAndStaminaWidget::OnAttributeChanged);

    Refresh();
}

void UHealthAndStaminaWidget::OnAttributeChanged(const FOnAttributeChangeData& InData)
{
    Refresh();
}

void UHealthAndStaminaWidget::Refresh()
{
    if (!AbilitySystemComponent.IsValid())
        return;

    const float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(CurrentHealthAttribute);
    const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(MaxHealthAttribute);

    CurrentHealthValue->SetText(FText::AsNumber(CurrentHealth));
    MaxHealthValue->SetText(FText::AsNumber(MaxHealth));
    HealthProgressBar->SetPercent(MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f);

    const float CurrentStamina = AbilitySystemComponent->GetNumericAttribute(CurrentStaminaAttribute);
    const float MaxStamina = AbilitySystemComponent->GetNumericAttribute(MaxStaminaAttribute);

    CurrentStaminaValue->SetText(FText::AsNumber(CurrentStamina));
    MaxStaminaValue->SetText(FText::AsNumber(MaxStamina));
    StaminaProgressBar->SetPercent(MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f);
}
