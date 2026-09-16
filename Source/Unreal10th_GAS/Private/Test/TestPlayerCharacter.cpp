// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestPlayerCharacter.h"
#include "Framework/TestGASHUD.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATestPlayerCharacter::ATestPlayerCharacter()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 400.0f;
    SpringArm->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void ATestPlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        // 플레이어 일때만 처리
        if (ATestGASHUD* HUD = Cast<ATestGASHUD>(PC->GetHUD()))
        {
            // 초기 실행 순서를 고려하여 안전하게 여기서도 HUD 초기화
            HUD->InitializeHUD(this);
        }
    }
}
