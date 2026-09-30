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
    virtual void OnRep_PlayerState() override;

protected:
    virtual void PossessedBy(AController* NewController) override;

};
