// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest04/NetTestCharacter04_PlayerState.h"
#include "Framework/TestGASHUD.h"
#include "Framework/TestPlayerState.h"
#include "Widget/PlayerNameWidget.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

ANetTestCharacter04_PlayerState::ANetTestCharacter04_PlayerState()
{
    PlayerNameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerNameWidgetComponent"));
    PlayerNameWidgetComponent->SetupAttachment(RootComponent);
    PlayerNameWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    PlayerNameWidgetComponent->SetDrawSize(FVector2D(450.0f, 50.0f));
    PlayerNameWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
    PlayerNameWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANetTestCharacter04_PlayerState::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    InitializePlayerNameWidget();

    // 클라이언트를 위한 HUD 초기화
    if (IsLocallyControlled())
    {
        InitializeLocalHUD();
    }

    //if (ATestPlayerState* TestPS = Cast<ATestPlayerState>(GetPlayerState()))
    //{
    //    if (UPlayerNameWidget* PlayerNameWidget = Cast<UPlayerNameWidget>(PlayerNameWidgetComponent->GetWidget()))
    //    {
    //        TestPS->OnNameChanged.AddUObject(PlayerNameWidget, &UPlayerNameWidget::UpdatePlayerName);
    //        PlayerNameWidget->UpdatePlayerName(TestPS->GetMyPlayerName());
    //    }
    //}
}

void ANetTestCharacter04_PlayerState::BeginPlay()
{
    Super::BeginPlay();

    InitializePlayerNameWidget();
}

void ANetTestCharacter04_PlayerState::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitializePlayerNameWidget();

    // 서버를 위한 HUD 초기화
    if (IsLocallyControlled())
    {
        InitializeLocalHUD();
    }

    //if (ATestPlayerState* TestPS = Cast<ATestPlayerState>(GetPlayerState()))
    //{
    //    if (UPlayerNameWidget* PlayerNameWidget = Cast<UPlayerNameWidget>(PlayerNameWidgetComponent->GetWidget()))
    //    {
    //        TestPS->OnNameChanged.AddUObject(PlayerNameWidget, &UPlayerNameWidget::UpdatePlayerName);
    //        PlayerNameWidget->UpdatePlayerName(TestPS->GetMyPlayerName());
    //    }
    //}
}

void ANetTestCharacter04_PlayerState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bFaceCamera)
    {
        UpdatePlayerNameWidgetRotation();
    }
}

void ANetTestCharacter04_PlayerState::UpdatePlayerNameWidgetRotation()
{
    if (!PlayerNameWidgetComponent)
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

        PlayerNameWidgetComponent->SetWorldRotation(WidgetRotation);
    }
}

void ANetTestCharacter04_PlayerState::InitializeLocalHUD()
{
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ATestGASHUD* HUD = Cast<ATestGASHUD>(PC->GetHUD()))
        {
            if (ATestPlayerState* TestPS = Cast<ATestPlayerState>(GetPlayerState()))
            {
                HUD->InitializeNetHUD(TestPS);
            }
        }
    }
}

void ANetTestCharacter04_PlayerState::InitializePlayerNameWidget()
{
    if (!PlayerNameWidgetComponent) { return; }

    PlayerNameWidgetComponent->InitWidget();

    ATestPlayerState* TestPS = GetPlayerState<ATestPlayerState>();
    UPlayerNameWidget* NameWidget = Cast<UPlayerNameWidget>(PlayerNameWidgetComponent->GetWidget());

    if (!TestPS || !NameWidget) { return; }

    // BeginPlay, PossessedBy, OnRep_PlayerState에서 중복 호출될 수 있음
    TestPS->OnNameChanged.RemoveAll(NameWidget);
    TestPS->OnNameChanged.AddUObject(NameWidget, &UPlayerNameWidget::UpdatePlayerName);

    // RepNotify가 이미 지나간 경우를 위해 현재 값으로 즉시 초기화
    NameWidget->UpdatePlayerName(TestPS->GetMyPlayerName());
}
