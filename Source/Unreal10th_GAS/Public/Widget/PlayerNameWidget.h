// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameWidget.generated.h"

class UTextBlock;

UCLASS()
class UNREAL10TH_GAS_API UPlayerNameWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void UpdatePlayerName(const FString& InNewName);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayerName;

};
