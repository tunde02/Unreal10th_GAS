// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UStatBarWidget;

UCLASS()
class UNREAL10TH_GAS_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindToCharacter(ACharacter* InCharacter);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UStatBarWidget> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UStatBarWidget> StaminaBarWidget;

};
