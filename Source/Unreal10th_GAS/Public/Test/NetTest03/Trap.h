// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class UNREAL10TH_GAS_API ATrap : public AActor
{
    GENERATED_BODY()

public:
    ATrap();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleOnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void HandleOnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_HitEffect(const FVector& InLocation, const FRotator& InRotator);

private:
    void ApplyDamage();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test|Trap")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test|Trap")
    TObjectPtr<UBoxComponent> BoxCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test|Trap")
    TObjectPtr<UNiagaraComponent> NiagaraComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|Trap")
    TObjectPtr<UNiagaraSystem> HitVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|Trap")
    float Damage = 5.0f;

private:
    TSet<TWeakObjectPtr<AActor>> OverlappingActors;
    FTimerHandle DamageTimerHandle;

};
