// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest03/PoisonTrap.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

APoisonTrap::APoisonTrap()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

    TrapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TrapCollision"));
    SetRootComponent(TrapCollision);
    TrapCollision->InitSphereRadius(TrapRadius);

    TrapEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrapEffectComponent"));
    TrapEffectComponent->SetupAttachment(TrapCollision);
    TrapEffectComponent->SetAutoActivate(true);
}

void APoisonTrap::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    TrapCollision->SetSphereRadius(TrapRadius);

    if (TrapEffectComponent)
    {
        TrapEffectComponent->SetFloatParameter(FName("Radius"), TrapRadius);
        TrapEffectComponent->SetColorParameter(FName("VFXColor"), ParticleColor);
    }
}

void APoisonTrap::BeginPlay()
{
    Super::BeginPlay();

    TrapCollision->OnComponentBeginOverlap.AddDynamic(this, &APoisonTrap::OnBeginOverlap);
    TrapCollision->OnComponentEndOverlap.AddDynamic(this, &APoisonTrap::OnEndOverlap);
}

void APoisonTrap::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (HasAuthority() && OtherActor != this)
    {
        DamageTargetActors.Add(OtherActor);

        FTimerManager& TimerManager = GetWorldTimerManager();
        if (!TimerManager.IsTimerActive(DamageTimerHandle))
        {
            TimerManager.SetTimer(
                DamageTimerHandle,
                this,
                &APoisonTrap::ApplyDamage,
                DamageInterval,
                true
            );
        }
    }
}

void APoisonTrap::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (HasAuthority() && OtherActor != this)
    {
        DamageTargetActors.Remove(OtherActor);

        if (DamageTargetActors.IsEmpty())
        {
            FTimerManager& TimerManager = GetWorldTimerManager();
            TimerManager.ClearTimer(DamageTimerHandle);
        }
    }
}

void APoisonTrap::Multicast_OnHit_Implementation(const FVector& InLocation)
{
    if (HitVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            HitVFX,
            InLocation,
            GetActorRotation(),
            FVector::OneVector,
            true,
            true,
            ENCPoolMethod::AutoRelease
        );
    }
}

void APoisonTrap::ApplyDamage()
{
    if (HasAuthority())
    {
        for (AActor* Target : DamageTargetActors)
        {
            if (IsValid(Target))
            {
                UGameplayStatics::ApplyDamage(Target, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
                Multicast_OnHit(Target->GetActorLocation());
            }
            else
            {
                DamageTargetActors.Remove(Target);
            }
        }
    }
}
