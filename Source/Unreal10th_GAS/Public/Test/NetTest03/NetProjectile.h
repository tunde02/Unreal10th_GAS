// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetProjectile.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class UNREAL10TH_GAS_API ANetProjectile : public AActor
{
    GENERATED_BODY()

public:
    ANetProjectile();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_HitEffect(const FVector& InLocation, const FRotator& InRotator);

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> Movement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
    TObjectPtr<UNiagaraSystem> HitVFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
    float Damage = 10.0f;

private:
    bool bHitted = false;

};
