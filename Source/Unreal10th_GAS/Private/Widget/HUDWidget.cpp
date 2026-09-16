// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUDWidget.h"
#include "Widget/StatWidget.h"

void UHUDWidget::InitializeWithAbilitySystem(AActor* InActor)
{
    if (StatWidget)
    {
        StatWidget->InitializeWithAbilitySystem(InActor);
    }
}
