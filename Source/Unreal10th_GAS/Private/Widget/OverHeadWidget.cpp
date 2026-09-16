// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/OverHeadWidget.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UOverHeadWidget::InitializeWithAbilitySystem(AActor* InActor)
{
    if (!InActor)
    {
        return;
    }

    IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(InActor);
    if (!ASI)
    {
        return;
    }

    ASC = ASI->GetAbilitySystemComponent();
    if (!ASC.IsValid())
    {
        return;
    }

    FOnGameplayAttributeValueChange& HealthChange = ASC->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetHealthAttribute());
    HealthChange.AddUObject(this, &UOverHeadWidget::OnHealthChanged);

    FOnGameplayAttributeValueChange& MaxHealthChange = ASC->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetMaxHealthAttribute());
    MaxHealthChange.AddUObject(this, &UOverHeadWidget::OnMaxHealthChanged);

    // 찾으면 bFound가 true, 못찾으면 false에 값은 0반환
    bool bFound = false;
    const float TempCurrent = ASC->GetGameplayAttributeValue(UStatAttributeSet::GetHealthAttribute(), bFound);
    CurrentHealth = bFound ? TempCurrent : 0.0f;

    bFound = false;
    const float TempMax = ASC->GetGameplayAttributeValue(UStatAttributeSet::GetHealthAttribute(), bFound);
    MaxHealth = bFound ? TempMax : 100.0f;

    UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UOverHeadWidget::OnHealthChanged(const FOnAttributeChangeData& InData)
{
    CurrentHealth = InData.NewValue;
    UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UOverHeadWidget::OnMaxHealthChanged(const FOnAttributeChangeData& InData)
{
    MaxHealth = InData.NewValue;
    UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UOverHeadWidget::UpdateHealthUI(float InCurrent, float InMax)
{
    if (HealthProgressBar)
    {
        const float Percent = FMath::IsNearlyZero(InMax) ? 0.0f : FMath::Clamp(InCurrent / InMax, 0.0f, 1.0f);
        HealthProgressBar->SetPercent(Percent);
    }
    if (HealthText)
    {
        HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), InCurrent, InMax)));
    }
}
