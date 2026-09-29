// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest03/NetProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ANetProjectile::ANetProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    SetReplicatingMovement(true); // 이 액터의 무브먼트 컴포넌트는 리플리케이션된다

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    Mesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->InitialSpeed = 1000.0f;
    Movement->MaxSpeed = 1000.0f;
    Movement->bShouldBounce = true;
}

void ANetProjectile::BeginPlay()
{
    Super::BeginPlay();

    OnActorHit.AddDynamic(this, &ANetProjectile::OnHit);

    if (GetInstigator())
    {
        Mesh->IgnoreActorWhenMoving(GetInstigator(), true);
    }
}

void ANetProjectile::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if (HasAuthority())
    {
        if (!bHitted
            && OtherActor->IsA<ACharacter>()
            && OtherActor != this
            && OtherActor != GetOwner())
        {
            bHitted = true;

            UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
            Multicast_HitEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
            SetLifeSpan(5.0f);

            const FString Str = FString::Printf(TEXT("%s가 %s를 공격했습니다."),
                                                GetInstigator() ? *GetInstigator()->GetName() : TEXT("없음"),
                                                *OtherActor->GetName());
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, Str);
        }
    }
}

void ANetProjectile::Multicast_HitEffect_Implementation(const FVector& InLocation, const FRotator& InRotator)
{
    if (HitVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, InLocation, InRotator);
    }
}
