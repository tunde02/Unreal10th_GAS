// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"
#include "StatBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;
//struct FGameplayAttribute;

UCLASS()
class UNREAL10TH_GAS_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindToAttribute(
		UAbilitySystemComponent* InAbilitySystemComponent,
		FGameplayAttribute InCurrentAttribute,
		FGameplayAttribute InMaxAttribute);

private:
	void OnAttributeChanged(const FOnAttributeChangeData& InData);
	void RefreshStatBar();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> StatProgressBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentStatValue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxStatValue;

private:
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	FGameplayAttribute CurrentAttribute;
	FGameplayAttribute MaxAttribute;

};
