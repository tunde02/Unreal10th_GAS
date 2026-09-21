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

        if (JumpAction)
        {
            EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ATestPlayerCharacter::OnJumpInputStart);
            EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATestPlayerCharacter::OnJumpInputCompleted);
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

void ATestPlayerCharacter::GiveDefaultAbilities()
{
    if (!AbilitySystemComponent || !AbilitySystemComponent->IsOwnerActorAuthoritative()) { return; }

    if (!AbilitySystemComponent->AbilityActorInfo.IsValid())
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    auto GiveIfMissing = [this](TSubclassOf<UGameplayAbility> AbilityClass, EDefaultAbilityInput Input)
    {
        if (!AbilityClass || AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass)) { return; }

        int32 InputId = INDEX_NONE;
        switch (Input)
        {
        case EDefaultAbilityInput::Sprint: InputId = SPRINT_INPUT_ID; break;
        case EDefaultAbilityInput::Jump: InputId = JUMP_INPUT_ID; break;
        default: break;
        }

        AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, DefaultAbilityLevel, InputId));
    };

    for (const FDefaultAbilityEntry& Entry : DefaultAbilities)
    {
        GiveIfMissing(Entry.AbilityClass, Entry.Input);
    }

    // 기존 블루프린트의 단일 어빌리티 설정을 계속 사용할 수 있게 한다.
    if (DefaultAbilities.IsEmpty())
    {
        GiveIfMissing(DefaultAbilityClass, EDefaultAbilityInput::Sprint);
    }
}

void ATestPlayerCharacter::OnSprintInputStart()
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->AbilityLocalInputPressed(SPRINT_INPUT_ID);
    }
}

void ATestPlayerCharacter::OnSprintInputCompleted()
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->AbilityLocalInputReleased(SPRINT_INPUT_ID);
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

void ATestPlayerCharacter::OnJumpInputStart()
{
    if (AbilitySystemComponent)
    {
        if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
        {
            MovementComp->JumpZVelocity = BaseJumpZVelocity;
        }

        AbilitySystemComponent->AbilityLocalInputPressed(JUMP_INPUT_ID);
    }
}

void ATestPlayerCharacter::OnJumpInputCompleted()
{
    if (AbilitySystemComponent)
    {
        if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
        {
            const float JumpGauge = AbilitySystemComponent->GetNumericAttribute(UStatAttributeSet::GetJumpGaugeAttribute());
            MovementComp->JumpZVelocity += JumpGauge * 10.0f;
        }

        AbilitySystemComponent->AbilityLocalInputReleased(JUMP_INPUT_ID);
    }

    Jump();
}
