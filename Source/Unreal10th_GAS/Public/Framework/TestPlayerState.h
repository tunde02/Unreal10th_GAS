// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TestPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNameChanged, const FString&)

UCLASS()
class UNREAL10TH_GAS_API ATestPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    void AddMyPlayerScore(int32 InPoint);
    int GetMyPlayerScore() const { return MyPlayerScore; }

    void SetMyPlayerName(const FString& InNewName);
    const FString& GetMyPlayerName() const { return MyPlayerName; }

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SetMyPlayerName(const FString& InNewName);

    UFUNCTION()
    void OnRepNotify_MyPlayerScore();

    UFUNCTION()
    void OnRepNotify_MyPlayerName();

public:
    FOnNameChanged OnNameChanged;

protected:
    UPROPERTY(ReplicatedUsing = OnRepNotify_MyPlayerScore, BlueprintReadOnly, Category = "Data")
    int32 MyPlayerScore = 0;

    UPROPERTY(ReplicatedUsing = OnRepNotify_MyPlayerName, BlueprintReadOnly, Category = "Data")
    FString MyPlayerName;

};
