// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/Test02/Test02_2.h"
#include "GameplayTagContainer.h"
#include "Test02_4.generated.h"

UCLASS()
class UNREAL10TH_GAS_API ATest02_4 : public ATest02_2
{
    GENERATED_BODY()

public:
    virtual void ApplyGameplayEffect() override;

    UFUNCTION(CallInEditor, Category = "GAS")
    void AddTag();

    UFUNCTION(CallInEditor, Category = "GAS")
    void RemoveAllTag();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
    TObjectPtr<ATestCharacter> Source;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (Categories = "GAS"))
    FGameplayTag TagToAdd;

};
