// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamagePopupWidget.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamagePopupFinished);

UCLASS()
class UNREAL10TH_GAS_API UDamagePopupWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void PlayPopup(float InDamage);

protected:
    UFUNCTION()
    void HandleAnimiationFinished();

public:
    FOnDamagePopupFinished OnDamagePopupFinished;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DamageText;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> PopupAnimation;

};
