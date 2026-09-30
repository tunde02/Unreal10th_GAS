// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreItemActor.generated.h"

class USphereComponent;

UCLASS()
class UNREAL10TH_GAS_API AScoreItemActor : public AActor
{
    GENERATED_BODY()

public:
    AScoreItemActor();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<USphereComponent> Collision;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Score = 10;

};
