// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DamagePopupWidget.h"

#include "Components/TextBlock.h"

void UDamagePopupWidget::PlayPopup(float InDamage)
{
    DamageText->SetText(FText::AsNumber(FMath::RoundToInt(InDamage)));

    FWidgetAnimationDynamicEvent Event;
    Event.BindDynamic(this, &UDamagePopupWidget::HandleAnimiationFinished);

    BindToAnimationFinished(PopupAnimation, Event);

    PlayAnimation(PopupAnimation);
}

void UDamagePopupWidget::HandleAnimiationFinished()
{
    OnDamagePopupFinished.Broadcast();
}
