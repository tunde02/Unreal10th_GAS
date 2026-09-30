// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerNameWidget.h"

#include "Components/TextBlock.h"

void UPlayerNameWidget::UpdatePlayerName(const FString& InNewName)
{
    PlayerName->SetText(FText::FromString(InNewName));
}
