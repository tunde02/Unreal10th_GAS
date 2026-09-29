// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest03/Trap.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

ATrap::ATrap()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetupAttachment(Mesh);

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(Mesh);
    NiagaraComponent->bAutoActivate = false;
}

void ATrap::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ATrap::HandleOnActorBeginOverlap);
    OnActorEndOverlap.AddDynamic(this, &ATrap::HandleOnActorEndOverlap);

    NiagaraComponent->Activate(true);
}

void ATrap::HandleOnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor != this
        && OtherActor->IsA<ACharacter>())
    {
        OverlappingActors.Add(OtherActor);

        if (!GetWorldTimerManager().IsTimerActive(DamageTimerHandle))
        {
            GetWorldTimerManager().SetTimer(
                DamageTimerHandle,
                this,
                &ATrap::ApplyDamage,
                1.0f,
                true
            );
        }
    }
}

void ATrap::HandleOnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    OverlappingActors.Remove(OtherActor);

    if (OverlappingActors.IsEmpty())
    {
        GetWorldTimerManager().ClearTimer(DamageTimerHandle);
    }
}

void ATrap::ApplyDamage()
{
    for (auto Target : OverlappingActors)
    {
        UGameplayStatics::ApplyDamage(Target.Get(), Damage, GetInstigatorController(), this, UDamageType::StaticClass());
        Multicast_HitEffect(Target->GetActorLocation(), FRotator::ZeroRotator);
    }
}

void ATrap::Multicast_HitEffect_Implementation(const FVector& InLocation, const FRotator& InRotator)
{
    if (HitVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, InLocation, InRotator);
    }
}
