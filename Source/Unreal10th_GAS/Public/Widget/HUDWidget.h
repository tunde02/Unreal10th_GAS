// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UStatWidget;

UCLASS()
class UNREAL10TH_GAS_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    virtual void InitializeWithAbilitySystem(AActor* InActor);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UStatWidget> StatWidget;

};
