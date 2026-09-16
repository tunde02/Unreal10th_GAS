// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"

ATestPlayerController::ATestPlayerController()
{
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextAsset(
        TEXT("/Game/Blueprints/Input/IMC_TestDefault.IMC_TestDefault"));
    static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionAsset(
        TEXT("/Game/_ExternalAssets/Input/Actions/IA_Jump.IA_Jump"));
    static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionAsset(
        TEXT("/Game/_ExternalAssets/Input/Actions/IA_Move.IA_Move"));
    static ConstructorHelpers::FObjectFinder<UInputAction> LookActionAsset(
        TEXT("/Game/_ExternalAssets/Input/Actions/IA_MouseLook.IA_MouseLook"));

    DefaultMappingContext = MappingContextAsset.Object;
    JumpAction = JumpActionAsset.Object;
    MoveAction = MoveActionAsset.Object;
    LookAction = LookActionAsset.Object;
}

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (DefaultMappingContext)
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ATestPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (JumpAction)
        {
            EnhancedInputComponent->BindAction(
                JumpAction, ETriggerEvent::Started, this, &ATestPlayerController::Jump);
            EnhancedInputComponent->BindAction(
                JumpAction, ETriggerEvent::Completed, this, &ATestPlayerController::StopJumping);
        }

        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(
                MoveAction, ETriggerEvent::Triggered, this, &ATestPlayerController::Move);
        }

        if (LookAction)
        {
            EnhancedInputComponent->BindAction(
                LookAction, ETriggerEvent::Triggered, this, &ATestPlayerController::Look);
        }
    }
}

void ATestPlayerController::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (APawn* ControlledPawn = GetPawn())
    {
        const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ATestPlayerController::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddYawInput(LookAxisVector.X);
    AddPitchInput(-LookAxisVector.Y);
}

void ATestPlayerController::Jump()
{
    if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
    {
        ControlledCharacter->Jump();
    }
}

void ATestPlayerController::StopJumping()
{
    if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
    {
        ControlledCharacter->StopJumping();
    }
}

