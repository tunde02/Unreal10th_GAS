// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/NetTest03/NetTestCharacter03_RPC.h"
#include "NetTestCharacter04_PlayerState.generated.h"

UCLASS()
class UNREAL10TH_GAS_API ANetTestCharacter04_PlayerState : public ANetTestCharacter03_RPC
{
    GENERATED_BODY()

public:
    ANetTestCharacter04_PlayerState();

    virtual void OnRep_PlayerState() override;

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void Tick(float DeltaTime) override;

    void UpdatePlayerNameWidgetRotation();

    void InitializeLocalHUD();
    void InitializePlayerNameWidget();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|OverHead")
    TObjectPtr<UWidgetComponent> PlayerNameWidgetComponent;

};
