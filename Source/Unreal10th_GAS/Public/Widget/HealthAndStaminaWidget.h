// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"
#include "HealthAndStaminaWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS()
class UNREAL10TH_GAS_API UHealthAndStaminaWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void BindToAttribute(
        UAbilitySystemComponent* InAbilitySystemComponent,
        FGameplayAttribute InCurrentHealthAttribute,
        FGameplayAttribute InMaxHealthAttribute,
        FGameplayAttribute InCurrentStaminaAttribute,
        FGameplayAttribute InMaxStaminaAttribute);

private:
    void OnAttributeChanged(const FOnAttributeChangeData& InData);
    void Refresh();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentHealthValue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> MaxHealthValue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UProgressBar> StaminaProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentStaminaValue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> MaxStaminaValue;

private:
    TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
    FGameplayAttribute CurrentHealthAttribute;
    FGameplayAttribute MaxHealthAttribute;
    FGameplayAttribute CurrentStaminaAttribute;
    FGameplayAttribute MaxStaminaAttribute;

};
