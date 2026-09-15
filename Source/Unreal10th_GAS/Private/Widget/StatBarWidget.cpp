// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StatBarWidget.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatBarWidget::BindToAttribute(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayAttribute InCurrentAttribute, FGameplayAttribute InMaxAttribute)
{
    if (!InAbilitySystemComponent)
        return;

    AbilitySystemComponent = InAbilitySystemComponent;
    CurrentAttribute = InCurrentAttribute;
    MaxAttribute = InMaxAttribute;

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentAttribute)
        .AddUObject(this, &UStatBarWidget::OnAttributeChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
        .AddUObject(this, &UStatBarWidget::OnAttributeChanged);

    RefreshStatBar();
}

void UStatBarWidget::OnAttributeChanged(const FOnAttributeChangeData& InData)
{
    RefreshStatBar();
}

void UStatBarWidget::RefreshStatBar()
{
    if (!AbilitySystemComponent.IsValid())
        return;

    const float Current = AbilitySystemComponent->GetNumericAttribute(CurrentAttribute);
    const float Max = AbilitySystemComponent->GetNumericAttribute(MaxAttribute);

    CurrentStatValue->SetText(FText::AsNumber(Current));
    MaxStatValue->SetText(FText::AsNumber(Max));

    StatProgressBar->SetPercent(Max > 0.f ? Current / Max : 0.f);
}
