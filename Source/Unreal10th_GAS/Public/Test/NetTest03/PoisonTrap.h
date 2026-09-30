// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoisonTrap.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class UNREAL10TH_GAS_API APoisonTrap : public AActor
{
    GENERATED_BODY()

public:
    APoisonTrap();

    virtual void OnConstruction(const FTransform& Transform) override;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_OnHit(const FVector& InLocation);

    void ApplyDamage();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> TrapCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraComponent> TrapEffectComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraSystem> HitVFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageInterval = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Damage = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TrapRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FLinearColor ParticleColor = FLinearColor::White;

private:
    TSet<AActor*> DamageTargetActors;
    FTimerHandle DamageTimerHandle;

};
