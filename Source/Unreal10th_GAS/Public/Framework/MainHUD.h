// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UMainWidget;

UCLASS()
class UNREAL10TH_GAS_API AMainHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UMainWidget> MainWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<UMainWidget> MainWidgetInstance;

};
