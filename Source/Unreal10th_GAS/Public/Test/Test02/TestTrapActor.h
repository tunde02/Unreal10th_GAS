// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "TestTrapActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UGameplayEffect;
class UStatAttributeSet;

UCLASS()
class UNREAL10TH_GAS_API ATestTrapActor : public AActor, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ATestTrapActor();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const { return ASC; }
    UStatAttributeSet* GetStatAttributeSet() const { return Stat; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
    void ApplyGameplayEffect(AActor* Target);
    void RemoveGameplayEffect(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UBoxComponent> BoxCollision = nullptr;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
    TSubclassOf<UGameplayEffect> GameplayEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
    float EffectLevel = 1.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UAbilitySystemComponent> ASC;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStatAttributeSet> Stat;

};
