// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestGASHUD.h"
#include "Widget/HUDWidget.h"
#include "Framework/TestPlayerState.h"

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

void ATestGASHUD::InitializeNetHUD(ATestPlayerState* InPS)
{
    // 반드시 InitializeHUD() 이후에 실행되어야 한다
    if (!HUDWidgetInstance) { return; }

    HUDWidgetInstance->InitializePlayerInfo(InPS);
}

void ATestGASHUD::BeginPlay()
{
    Super::BeginPlay();

    if (APawn* Pawn = GetOwningPawn())
    {
        InitializeHUD(Pawn);
    }
}
