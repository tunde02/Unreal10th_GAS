// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestPlayerCharacter.h"
#include "Framework/TestGASHUD.h"
#include "GAS/StatAttributeSet.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"

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

    if (!MoveSpeedChangedDelegateHandle.IsValid())
    {
        FOnGameplayAttributeValueChange& MoveSpeedChangedDelegate =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetMoveSpeedAttribute());
        MoveSpeedChangedDelegateHandle = MoveSpeedChangedDelegate.AddUObject(this, &ATestPlayerCharacter::OnMoveSpeedChanged);
    }

    if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
    {
        const float Ratio = (StatAttributeSet ? StatAttributeSet->GetMoveSpeed() : 100.0f) / 100.0f;
        MovementComp->MaxWalkSpeed = BaseWalkSpeed * Ratio;
    }

    static const FGameplayTag GroundedTag = FGameplayTag::RequestGameplayTag(FName("GAS.State.Grounded"), false);
    if (GroundedTag.IsValid())
    {
        if (GetCharacterMovement()->IsMovingOnGround())
        {
            if (!AbilitySystemComponent->HasMatchingGameplayTag(GroundedTag))
            {
                AbilitySystemComponent->AddLooseGameplayTag(GroundedTag);
            }
        }
        else
        {
            if (AbilitySystemComponent->HasMatchingGameplayTag(GroundedTag))
            {
                AbilitySystemComponent->RemoveLooseGameplayTag(GroundedTag);
            }
        }
    }

    if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        // 플레이어 일때만 처리
        if (ATestGASHUD* HUD = Cast<ATestGASHUD>(PC->GetHUD()))
        {
            // 초기 실행 순서를 고려하여 안전하게 여기서도 HUD 초기화
            HUD->InitializeHUD(this);
        }
    }

    GiveDefaultAbilities();
}

void ATestPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (SprintAction)
        {
            EnhancedInputComp->BindAction(SprintAction, ETriggerEvent::Started, this, &ATestPlayerCharacter::OnSprintInputStart);
            EnhancedInputComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATestPlayerCharacter::OnSprintInputCompleted);
        }

        if (ChargeJumpAction)
        {
            EnhancedInputComp->BindAction(ChargeJumpAction, ETriggerEvent::Started, this, &ATestPlayerCharacter::OnChargeJumpInputStart);
            EnhancedInputComp->BindAction(ChargeJumpAction, ETriggerEvent::Completed, this, &ATestPlayerCharacter::OnChargeJumpInputCompleted);
        }
    }
}

void ATestPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!AbilitySystemComponent) { return; }

    static const FGameplayTag StateMovingTag = FGameplayTag::RequestGameplayTag(FName("GAS.State.Moving"), false);
    const bool bIsMoving = GetVelocity().SizeSquared2D() >= FMath::Square(MoveThreshold);
    const bool bHasMovingTag = AbilitySystemComponent->HasMatchingGameplayTag(StateMovingTag);

    if (bIsMoving && !bHasMovingTag)
    {
        AbilitySystemComponent->AddLooseGameplayTag(StateMovingTag);
    }
    else if (!bIsMoving && bHasMovingTag)
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(StateMovingTag);
    }
}

void ATestPlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

    if (!AbilitySystemComponent) { return; }

    static const FGameplayTag GroundedTag = FGameplayTag::RequestGameplayTag(FName("GAS.State.Grounded"), false);
    if (!GroundedTag.IsValid()) { return; }

    if (GetCharacterMovement()->IsMovingOnGround())
    {
        if (!AbilitySystemComponent->HasMatchingGameplayTag(GroundedTag))
        {
            AbilitySystemComponent->AddLooseGameplayTag(GroundedTag);
        }
    }
    else
    {
        if (AbilitySystemComponent->HasMatchingGameplayTag(GroundedTag))
        {
            AbilitySystemComponent->RemoveLooseGameplayTag(GroundedTag);
        }
    }
}

void ATestPlayerCharacter::GiveDefaultAbilities()
{
    if (!AbilitySystemComponent) { return; }

    if (!AbilitySystemComponent->AbilityActorInfo.IsValid())
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    if (DefaultAbilityClass)
    {
        FGameplayAbilitySpec Spec(DefaultAbilityClass, DefaultAbilityLevel, SprintInputId);
        SprintAbilitySpecHandle = AbilitySystemComponent->GiveAbility(Spec);
    }

    if (DefaultJumpAbilityClass)
    {
        FGameplayAbilitySpec Spec(DefaultJumpAbilityClass, DefaultAbilityLevel, ChargeJumpInputId);
        JumpAbilitySpecHandle = AbilitySystemComponent->GiveAbility(Spec);
    }
}

void ATestPlayerCharacter::OnSprintInputStart()
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->AbilityLocalInputPressed(SprintInputId);
    }
}

void ATestPlayerCharacter::OnSprintInputCompleted()
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->AbilityLocalInputReleased(SprintInputId);
    }
}

void ATestPlayerCharacter::OnChargeJumpInputStart()
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->AbilityLocalInputPressed(ChargeJumpInputId);
    }
}

void ATestPlayerCharacter::OnChargeJumpInputCompleted()
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->AbilityLocalInputReleased(ChargeJumpInputId);
    }
}

void ATestPlayerCharacter::OnMoveSpeedChanged(const FOnAttributeChangeData& InData)
{
    if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
    {
        const float Ratio = InData.NewValue / 100.0f;
        MovementComp->MaxWalkSpeed = BaseWalkSpeed * Ratio;
    }
}
