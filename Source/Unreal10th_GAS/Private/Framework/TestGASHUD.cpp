// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestGASHUD.h"
#include "Widget/HUDWidget.h"

#include "Blueprint/UserWidget.h"

void ATestGASHUD::InitializeHUD(APawn* InPawn)
{
    if (!InPawn)
    {
        return;
    }

    if (!HUDWidgetClass)
    {
        return;
    }

    APlayerController* PC = GetOwningPlayerController();
    if (!PC)
    {
        return;
    }

    if (!HUDWidgetInstance)
    {
        HUDWidgetInstance = CreateWidget<UHUDWidget>(PC, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
            HUDWidgetInstance->InitializeWithAbilitySystem(InPawn);
        }
    }
}

void ATestGASHUD::BeginPlay()
{
    Super::BeginPlay();

    if (APawn* Pawn = GetOwningPawn())
    {
        InitializeHUD(Pawn);
    }
}
