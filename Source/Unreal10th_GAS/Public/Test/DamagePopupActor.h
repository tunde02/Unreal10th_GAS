// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagePopupActor.generated.h"

class UWidgetComponent;

UCLASS()
class UNREAL10TH_GAS_API ADamagePopupActor : public AActor
{
    GENERATED_BODY()

public:
    ADamagePopupActor();

    void Initialize(float InDamage);

protected:
    UFUNCTION()
    void OnPopupFinished();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UWidgetComponent> WidgetComponent;

};
