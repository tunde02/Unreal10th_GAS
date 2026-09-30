// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

class ATestPlayerState;
class UTextBlock;

UCLASS()
class UNREAL10TH_GAS_API UPlayerInfoWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitializePlayerStateBind(ATestPlayerState* InPS);

protected:
    void UpdatePlayerName(const FString& InName);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayerName;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayerScore;

};
