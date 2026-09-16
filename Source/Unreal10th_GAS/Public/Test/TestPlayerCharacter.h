// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/TestCharacter.h"
#include "TestPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UNREAL10TH_GAS_API ATestPlayerCharacter : public ATestCharacter
{
    GENERATED_BODY()

public:
    ATestPlayerCharacter();

protected:
    virtual void PossessedBy(AController* NewController) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

};
