// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestEnemyCharacter.h"
#include "Widget/OverHeadWidget.h"

#include "Components/WidgetComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

ATestEnemyCharacter::ATestEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidgetComponent"));
    OverHeadWidgetComponent->SetupAttachment(RootComponent);
    OverHeadWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    OverHeadWidgetComponent->SetDrawSize(FVector2D(150.0f, 20.0f));
    OverHeadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    OverHeadWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATestEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(AbilitySystemComponent))
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
        InitializeOverHeadWidget();
    }
}

void ATestEnemyCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    //InitializeOverHeadWidget();
}

void ATestEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bFaceCamera)
    {
        UpdateOverHeadWidgetRotation();
    }
}

void ATestEnemyCharacter::InitializeOverHeadWidget()
{
    if (!OverHeadWidgetComponent)
    {
        return;
    }

    if (UUserWidget* UserWidget = OverHeadWidgetComponent->GetUserWidgetObject())
    {
        if (UOverHeadWidget* OverHeadWidget = Cast<UOverHeadWidget>(UserWidget))
        {

            OverHeadWidget->InitializeWithAbilitySystem(this);
        }
    }
}

void ATestEnemyCharacter::UpdateOverHeadWidgetRotation()
{
    if (!OverHeadWidgetComponent)
    {
        return;
    }

    if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
    {
        FRotator WidgetRotation = (-CameraManager->GetCameraRotation().Vector()).Rotation();

        if (bLockWidgetPitch)
        {
            WidgetRotation.Pitch = 0.0f;
        }

        if (bLockWidgetRoll)
        {
            WidgetRotation.Roll = 0.0f;
        }

        OverHeadWidgetComponent->SetWorldRotation(WidgetRotation);
    }
}
