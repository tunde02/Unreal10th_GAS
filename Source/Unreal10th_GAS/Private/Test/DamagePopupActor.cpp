// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DamagePopupActor.h"
#include "Test/DamagePopupWidget.h"

#include "Components/WidgetComponent.h"

ADamagePopupActor::ADamagePopupActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    SetRootComponent(WidgetComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

void ADamagePopupActor::Initialize(float InDamage)
{
    if (UDamagePopupWidget* Widget = Cast<UDamagePopupWidget>(WidgetComponent->GetUserWidgetObject()))
    {
        Widget->OnDamagePopupFinished.AddDynamic(this, &ADamagePopupActor::OnPopupFinished);
        Widget->PlayPopup(InDamage);
    }
}

void ADamagePopupActor::OnPopupFinished()
{
    Destroy();
}

