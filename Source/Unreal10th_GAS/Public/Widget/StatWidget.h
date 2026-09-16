// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/OverHeadWidget.h"
#include "GameplayEffectTypes.h"
#include "StatWidget.generated.h"

UCLASS()
class UNREAL10TH_GAS_API UStatWidget : public UOverHeadWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeWithAbilitySystem(AActor* InActor) override;

protected:
    virtual void OnStaminaChanged(const FOnAttributeChangeData& InData);
    virtual void OnMaxStaminaChanged(const FOnAttributeChangeData& InData);
    virtual void UpdateStaminaUI(float InCurrent, float InMax);

    // C++에서는 선언만 하고, 블루프린트에서 내부를 구현할 수 있음
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStaminaChanged"))
    void BP_OnStaminaChanged(float InCurrent, float InMax, float InPercent);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UProgressBar> StaminaProgressBar;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> StaminaText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentStamina = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxStamina = 0.0f;

};
