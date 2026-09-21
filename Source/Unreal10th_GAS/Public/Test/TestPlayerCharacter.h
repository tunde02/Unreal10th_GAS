// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/TestCharacter.h"
#include "GameplayAbilitySpecHandle.h"
#include "TestPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UGameplayAbility;

UENUM(BlueprintType)
enum class EDefaultAbilityInput : uint8
{
    None,
    Sprint,
    Jump
};

USTRUCT(BlueprintType)
struct FDefaultAbilityEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    TSubclassOf<UGameplayAbility> AbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    EDefaultAbilityInput Input = EDefaultAbilityInput::None;
};

UCLASS()
class UNREAL10TH_GAS_API ATestPlayerCharacter : public ATestCharacter
{
    GENERATED_BODY()

public:
    ATestPlayerCharacter();

protected:
    virtual void PossessedBy(AController* NewController) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

    virtual void GiveDefaultAbilities();

    // 스프린트 입력 시작 콜백
    void OnSprintInputStart();

    // 스프린트 입력 종료 콜백
    void OnSprintInputCompleted();

    // MoveSpeed 어트리뷰트 변경 콜백
    void OnMoveSpeedChanged(const struct FOnAttributeChangeData& InData);

    // 점프 입력 시작 콜백
    void OnJumpInputStart();

    // 점프 입력 종료 콜백
    void OnJumpInputCompleted();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float BaseWalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float MoveThreshold = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (DeprecatedProperty, DeprecationMessage = "Use DefaultAbilities"))
    TSubclassOf<UGameplayAbility> DefaultAbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TArray<FDefaultAbilityEntry> DefaultAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (Clamp = "1"))
    int32 DefaultAbilityLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
    float BaseJumpZVelocity = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;

private:
    UPROPERTY(Transient)
    FGameplayAbilitySpecHandle SprintAbilitySpecHandle;

    UPROPERTY(Transient)
    FGameplayAbilitySpecHandle JumpAbilitySpecHandle;

    FDelegateHandle MoveSpeedChangedDelegateHandle;

    static constexpr int32 SPRINT_INPUT_ID = 100;
    static constexpr int32 JUMP_INPUT_ID = 101;

};
