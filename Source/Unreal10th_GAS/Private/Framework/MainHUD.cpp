// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainHUD.h"
#include "Widget/MainWidget.h"

void AMainHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MainWidgetClass)
    {
        MainWidgetInstance = CreateWidget<UMainWidget>(GetWorld(), MainWidgetClass);
        if (MainWidgetInstance)
        {
            MainWidgetInstance->AddToViewport();
            MainWidgetInstance->BindToCharacter(GetWorld()->GetFirstPlayerController()->GetCharacter());
        }
    }
}
