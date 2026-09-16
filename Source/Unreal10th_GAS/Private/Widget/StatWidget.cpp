// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StatWidget.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatWidget::InitializeWithAbilitySystem(AActor* InActor)
{
    Super::InitializeWithAbilitySystem(InActor);

    if (!ASC.IsValid())
    {
        return;
    }

    FOnGameplayAttributeValueChange& StaminaChange = ASC->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetStaminaAttribute());
    StaminaChange.AddUObject(this, &UStatWidget::OnStaminaChanged);

    FOnGameplayAttributeValueChange& MaxStaminaChange = ASC->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetMaxStaminaAttribute());
    MaxStaminaChange.AddUObject(this, &UStatWidget::OnMaxStaminaChanged);

    // 찾으면 bFound가 true, 못찾으면 false에 값은 0반환
    bool bFound = false;
    const float TempCurrent = ASC->GetGameplayAttributeValue(UStatAttributeSet::GetStaminaAttribute(), bFound);
    CurrentStamina = bFound ? TempCurrent : 0.0f;

    bFound = false;
    const float TempMax = ASC->GetGameplayAttributeValue(UStatAttributeSet::GetStaminaAttribute(), bFound);
    MaxStamina = bFound ? TempMax : 100.0f;

    UpdateStaminaUI(CurrentStamina, MaxStamina);
}

void UStatWidget::OnStaminaChanged(const FOnAttributeChangeData& InData)
{
    CurrentStamina = InData.NewValue;
    UpdateStaminaUI(CurrentStamina, MaxStamina);
}

void UStatWidget::OnMaxStaminaChanged(const FOnAttributeChangeData& InData)
{
    MaxStamina = InData.NewValue;
    UpdateStaminaUI(CurrentStamina, MaxStamina);
}

void UStatWidget::UpdateStaminaUI(float InCurrent, float InMax)
{
    if (StaminaProgressBar)
    {
        const float Percent = FMath::IsNearlyZero(InMax) ? 0.0f : FMath::Clamp(InCurrent / InMax, 0.0f, 1.0f);
        StaminaProgressBar->SetPercent(Percent);
    }
    if (StaminaText)
    {
        StaminaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), InCurrent, InMax)));
    }
}
