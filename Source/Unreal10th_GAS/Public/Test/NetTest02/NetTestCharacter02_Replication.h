// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/Test03/TestPlayerCharacter03.h"
#include "NetTestCharacter02_Replication.generated.h"

class UInputMappingContext;
class UInputAction;
class UWidgetComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS()
class UNREAL10TH_GAS_API ANetTestCharacter02_Replication : public ATestPlayerCharacter03
{
    GENERATED_BODY()

public:
    ANetTestCharacter02_Replication();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRepNotify_Level();

    UFUNCTION()
    void OnRepNotify_Health();

    UFUNCTION(CallInEditor, Category = "Test")
    void TestLevelUp();

    void UpdateOverHeadWidgetRotation();

    void SetHealth(float NewHealth);

    UFUNCTION()
    virtual void Test1();

    UFUNCTION()
    virtual void Test2();

    UFUNCTION()
    virtual void Test3();

public:
    FOnHealthChanged OnHealthChanged;

protected:
    // Level이 리플리케이션 될 때 OnRepNotify_Level 함수가 실행됨
    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRepNotify_Level)
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, Replicated)
    float Exp = 0.0f;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRepNotify_Health)
    float Health = 100.0f;

    UPROPERTY(VisibleAnywhere, Replicated)
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Input")

    TObjectPtr<UInputMappingContext> TestMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Test1;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Test2;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Test3;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|OverHead")
    TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|OverHead")
    bool bFaceCamera = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|OverHead")
    bool bLockWidgetPitch = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|OverHead")
    bool bLockWidgetRoll = true;

};
