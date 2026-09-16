// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestGASHUD.generated.h"

class UHUDWidget;

UCLASS()
class UNREAL10TH_GAS_API ATestGASHUD : public AHUD
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void InitializeHUD(APawn* InPawn);

    UFUNCTION(BlueprintCallable)
    UHUDWidget* GetHUDWidget() const { return HUDWidgetInstance; }

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UHUDWidget> HUDWidgetClass;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UHUDWidget> HUDWidgetInstance;

};
