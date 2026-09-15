// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/TestCharacter.h"
#include "PlayerTestCharacter.generated.h"

UCLASS()
class UNREAL10TH_GAS_API APlayerTestCharacter : public ATestCharacter
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ModifyHealth(float InAmount);

	UFUNCTION(BlueprintCallable)
	void ModifyStamina(float InAmount);

};
