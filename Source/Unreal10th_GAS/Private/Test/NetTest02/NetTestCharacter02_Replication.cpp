// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest02/NetTestCharacter02_Replication.h"
#include "Widget/OverHeadWidget.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

ANetTestCharacter02_Replication::ANetTestCharacter02_Replication()
{
    OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidgetComponent"));
    OverHeadWidgetComponent->SetupAttachment(RootComponent);
    OverHeadWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    OverHeadWidgetComponent->SetDrawSize(FVector2D(150.0f, 20.0f));
    OverHeadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    OverHeadWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANetTestCharacter02_Replication::BeginPlay()
{
    Super::BeginPlay();

    InitializeOverHeadWidget();
}

void ANetTestCharacter02_Replication::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bFaceCamera)
    {
        UpdateOverHeadWidgetRotation();
    }
}

void ANetTestCharacter02_Replication::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (TestMappingContext)
            {
                Subsystem->AddMappingContext(TestMappingContext, 1);
            }
        }
    }
}

void ANetTestCharacter02_Replication::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComp->BindAction(IA_Test1, ETriggerEvent::Started, this, &ANetTestCharacter02_Replication::Test1);
        EnhancedInputComp->BindAction(IA_Test2, ETriggerEvent::Started, this, &ANetTestCharacter02_Replication::Test2);
        EnhancedInputComp->BindAction(IA_Test3, ETriggerEvent::Started, this, &ANetTestCharacter02_Replication::Test3);
    }
}

void ANetTestCharacter02_Replication::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ANetTestCharacter02_Replication, Level);
    DOREPLIFETIME(ANetTestCharacter02_Replication, Exp);
    DOREPLIFETIME(ANetTestCharacter02_Replication, Health);
}

void ANetTestCharacter02_Replication::OnRepNotify_Level()
{
    const FString Str = FString::Printf(TEXT("서버에서 레벨을 %d로 변경했다고 알리고 있습니다."), Level);
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Str);
}

void ANetTestCharacter02_Replication::OnRepNotify_Health()
{
    //const FString Str = FString::Printf(TEXT("서버에서 체력을 %.1f로 변경했다고 알리고 있습니다."), Health);
    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Str);
    RefreshHealthUI();
}

void ANetTestCharacter02_Replication::TestLevelUp()
{
    if (HasAuthority())
    {
        Level++;
    }
}

void ANetTestCharacter02_Replication::InitializeOverHeadWidget()
{
    if (!OverHeadWidgetComponent)
    {
        return;
    }

    if (UUserWidget* UserWidget = OverHeadWidgetComponent->GetUserWidgetObject())
    {
        if (UOverHeadWidget* OverHeadWidget = Cast<UOverHeadWidget>(UserWidget))
        {
            OverHeadWidget->UpdateHealthUI(Health, MaxHealth);
            //UE_LOG(LogTemp, Log, TEXT("Hello %s"), *GetName());
        }
    }
}

void ANetTestCharacter02_Replication::UpdateOverHeadWidgetRotation()
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

void ANetTestCharacter02_Replication::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    RefreshHealthUI();
}

void ANetTestCharacter02_Replication::Test1()
{
    if (HasAuthority())
    {
        Level++;
    }
}

void ANetTestCharacter02_Replication::Test2()
{
    if (HasAuthority())
    {
        Exp += 10.0f;
    }
}

void ANetTestCharacter02_Replication::Test3()
{
    if (HasAuthority())
    {
        SetHealth(Health - 5.0f);
    }
}

void ANetTestCharacter02_Replication::RefreshHealthUI()
{
    if (!OverHeadWidgetComponent) { return; }

    if (UUserWidget* UserWidget = OverHeadWidgetComponent->GetUserWidgetObject())
    {
        if (UOverHeadWidget* OverHeadWidget = Cast<UOverHeadWidget>(UserWidget))
        {
            OverHeadWidget->UpdateHealthUI(Health, MaxHealth);
        }
    }
}
