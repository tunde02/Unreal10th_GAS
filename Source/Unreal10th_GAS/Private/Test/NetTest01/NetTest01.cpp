// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest01/NetTest01.h"
#include "Test/NetTest01/NetTestCharacter01_Connection.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ANetTest01::ANetTest01()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    OverlapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapCollision"));
    OverlapCollision->SetupAttachment(RootComponent);
    OverlapCollision->SetSphereRadius(400.0f);
}

void ANetTest01::BeginPlay()
{
    Super::BeginPlay();
}

void ANetTest01::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), OverlapCollision->GetScaledSphereRadius(), 23, FColor::Yellow);

    if (HasAuthority())
    {
        AActor* NextOwner = nullptr;
        float MinDistanceSquared = OverlapCollision->GetScaledSphereRadius() * OverlapCollision->GetScaledSphereRadius();
        TArray<AActor*> OverlapActors;

        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetTestCharacter01_Connection::StaticClass(), OverlapActors);
        for (AActor* Actor : OverlapActors)
        {
            float DistanceSquared = GetSquaredDistanceTo(Actor);
            if (MinDistanceSquared > DistanceSquared)
            {
                MinDistanceSquared = DistanceSquared;
                NextOwner = Actor;
            }
        }

        if (GetOwner() != NextOwner)
        {
            SetOwner(NextOwner);
            const FString OwnerName = GetOwner() ? GetOwner()->GetName() : TEXT("오너 없음");
            UE_LOG(LogTemp, Log, TEXT("새 오너 : %s"), *OwnerName);
        }
    }

    const FString LocalRoleString = UEnum::GetValueAsString(GetLocalRole());
    const FString RemoteRoleString = UEnum::GetValueAsString(GetRemoteRole());

    const FString OwnerString = GetOwner() ? GetOwner()->GetName() : TEXT("오너 없음");
    const FString ConnectionString = GetNetConnection() ? TEXT("커넥션 있음") : TEXT("커넥션 없음");
    const FString NetInfo = FString::Printf(TEXT("Owner : %s\nConnection : %s\nLocalRole : %s\nRemoteRole : %s"),
                                            *OwnerString, *ConnectionString, *LocalRoleString, *RemoteRoleString);

    DrawDebugString(GetWorld(), GetActorLocation(), NetInfo, nullptr, FColor::White, 0.0f, true);
}

void ANetTest01::ApplyTargetToOwner()
{
    if (!Target) { return; }

    if (HasAuthority())
    {
        SetOwner(Target);
        UE_LOG(LogTemp, Log, TEXT("%s가 오너로 설정되었습니다."), *Target->GetName());
    }
}
