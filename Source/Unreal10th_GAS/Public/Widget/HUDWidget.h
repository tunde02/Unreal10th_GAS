// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class ATestPlayerState;
class UStatWidget;
class UPlayerInfoWidget;
class UNameInputWidget;

UCLASS()
class UNREAL10TH_GAS_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    virtual void InitializeWithAbilitySystem(AActor* InActor);

    UFUNCTION(BlueprintCallable)
    virtual void InitializePlayerInfo(ATestPlayerState* InPS);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UStatWidget> StatWidget;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UPlayerInfoWidget> PlayerInfo;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UNameInputWidget> NameInput;

};
