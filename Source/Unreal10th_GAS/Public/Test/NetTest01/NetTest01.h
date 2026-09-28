// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetTest01.generated.h"

class USphereComponent;

UCLASS()
class UNREAL10TH_GAS_API ANetTest01 : public AActor
{
    GENERATED_BODY()

public:
    ANetTest01();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(CallInEditor, Category = "Test|Connection")
    void ApplyTargetToOwner();

protected:
    UPROPERTY(EditInstanceOnly, Category = "Test|Connection")
    TObjectPtr<ACharacter> Target;

    UPROPERTY(VisibleAnywhere, Category = "Test|Owner")
    TObjectPtr<USphereComponent> OverlapCollision;

};
