// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest04/ScoreItemActor.h"
#include "Framework/TestPlayerState.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

AScoreItemActor::AScoreItemActor()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetupAttachment(RootComponent);
}

void AScoreItemActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (!HasAuthority()) { return; }

    if (!OtherActor->IsA<ACharacter>()) { return; }

    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character) { return; }

    ATestPlayerState* TestPS = Cast<ATestPlayerState>(Character->GetPlayerState());
    if (!TestPS) { return; }

    TestPS->AddMyPlayerScore(Score);

    Destroy();
}
