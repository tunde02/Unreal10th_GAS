// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerInfoWidget.h"
#include "Framework/TestPlayerState.h"

#include "Components/TextBlock.h"

void UPlayerInfoWidget::InitializePlayerStateBind(ATestPlayerState* InPS)
{
    if (InPS)
    {
        InPS->OnNameChanged.AddUObject(this, &UPlayerInfoWidget::UpdatePlayerName);
        InPS->OnScoreChanged.AddUObject(this, &UPlayerInfoWidget::UpdatePlayerScore);

        UpdatePlayerName(InPS->GetMyPlayerName());
        UpdatePlayerScore(InPS->GetMyPlayerScore());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[UPlayerInfoWidget::InitializePlayerStateBind()] : PlayerState가 없습니다."));
    }
}

void UPlayerInfoWidget::UpdatePlayerName(const FString& InName)
{
    PlayerName->SetText(FText::FromString(InName));
}

void UPlayerInfoWidget::UpdatePlayerScore(int32 InScore)
{
    PlayerScore->SetText(FText::AsNumber(InScore));
}
