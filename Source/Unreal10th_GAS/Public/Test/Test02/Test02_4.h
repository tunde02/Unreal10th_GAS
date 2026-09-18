// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/Test02/Test02_1.h"
#include "Test02_4.generated.h"

UCLASS()
class UNREAL10TH_GAS_API ATest02_4 : public ATest02_1
{
    GENERATED_BODY()

public:
    virtual void ApplyGameplayEffect() override;

    UFUNCTION(CallInEditor, Category = "GAS")
    void ApplyCurseEffect();

    UFUNCTION(CallInEditor, Category = "GAS")
    void ApplyUndeadEffect();

    UFUNCTION(CallInEditor, Category = "GAS")
    void SetHealthToMax();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
    TObjectPtr<ATestCharacter> Source;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
    TSubclassOf<UGameplayEffect> CurseEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
    TSubclassOf<UGameplayEffect> UndeadEffectClass;

};
