// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/NetTest02/NetTestCharacter02_Replication.h"
#include "NetTestCharacter03_RPC.generated.h"

class UCameraShakeBase;
class UNiagaraSystem;
class ADamagePopupActor;

UCLASS()
class UNREAL10TH_GAS_API ANetTestCharacter03_RPC : public ANetTestCharacter02_Replication
{
    GENERATED_BODY()

public:
    ANetTestCharacter03_RPC();

protected:
    virtual void BeginPlay() override;
    virtual void Test1() override;

    UFUNCTION()
    void OnTakeDamage(
        AActor* DamagedActor,
        float Damage,
        const class UDamageType* DamageType,
        class AController* InstigatedBy,
        AActor* DamageCauser);

    // 서버에게 발사를 요청하는 함수
    UFUNCTION(Server, Reliable)
    void Server_Fire();

    // 특정 클라이언트가 발사체를 맞았을 때
    // 그 클라이언트 전용 효과를 재생하는 함수
    UFUNCTION(Client, Unreliable)
    void Client_OnHit(float InDamage);

private:
    // 실제로 발사체를 발사하는 함수
    void Fire();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|RPC")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|RPC")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|RPC")
    TObjectPtr<UNiagaraSystem> HitVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|RPC")
    TSubclassOf<ADamagePopupActor> DamagePopupClass;

};
