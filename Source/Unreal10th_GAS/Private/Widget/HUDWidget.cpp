// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUDWidget.h"
#include "Widget/StatWidget.h"
#include "Widget/PlayerInfoWidget.h"
#include "Framework/TestPlayerState.h"

void UHUDWidget::InitializeWithAbilitySystem(AActor* InActor)
{
    if (StatWidget)
    {
        StatWidget->InitializeWithAbilitySystem(InActor);
    }
}

void UHUDWidget::InitializePlayerInfo(ATestPlayerState* InPS)
{
    if (PlayerInfo)
    {
        PlayerInfo->InitializePlayerStateBind(InPS);
    }
}