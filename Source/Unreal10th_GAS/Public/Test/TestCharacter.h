// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "TestCharacter.generated.h"

class UAbilitySystemComponent;
class UStatAttributeSet;

UCLASS()
class UNREAL10TH_GAS_API ATestCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ATestCharacter();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UStatAttributeSet* GetStatAttribute() const;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PossessedBy(AController* NewController) override;

    //virtual void OnHealthChanged(const FOnAttributeChangeData& InData);
    //virtual void OnStaminaChanged(const FOnAttributeChangeData& InData);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
    TObjectPtr<UStatAttributeSet> StatAttributeSet;

};
