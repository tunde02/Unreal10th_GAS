// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/TestCharacter.h"
#include "TestEnemyCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class UNREAL10TH_GAS_API ATestEnemyCharacter : public ATestCharacter
{
    GENERATED_BODY()

public:
    ATestEnemyCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void InitializeOverHeadWidget();

    void UpdateOverHeadWidgetRotation();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OverHead")
    TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|OverHead")
    bool bFaceCamera = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|OverHead")
    bool bLockWidgetPitch = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|OverHead")
    bool bLockWidgetRoll = true;

};
