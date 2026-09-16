// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/TestCharacter.h"
#include "EnemyTestCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class UNREAL10TH_GAS_API AEnemyTestCharacter : public ATestCharacter
{
    GENERATED_BODY()

public:
    AEnemyTestCharacter();

    UFUNCTION(BlueprintCallable)
    void ModifyHealth(float InAmount);

    UFUNCTION(BlueprintCallable)
    void ModifyStamina(float InAmount);

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatBar")
    TObjectPtr<UWidgetComponent> WidgetComponent;

};
