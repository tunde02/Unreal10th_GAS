// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetTest03/NetTestCharacter03_RPC.h"
#include "Test/DamagePopupActor.h"

#include "NiagaraFunctionLibrary.h"

ANetTestCharacter03_RPC::ANetTestCharacter03_RPC()
{
}

void ANetTestCharacter03_RPC::BeginPlay()
{
    Super::BeginPlay();

    OnTakeAnyDamage.AddDynamic(this, &ANetTestCharacter03_RPC::OnTakeDamage);
}

void ANetTestCharacter03_RPC::Test1()
{
    Fire();
}

void ANetTestCharacter03_RPC::OnTakeDamage(
    AActor* DamagedActor,
    float Damage,
    const UDamageType* DamageType,
    AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (HasAuthority())
    {
        Health -= Damage;
        OnRepNotify_Health();

        // 맞은 클라이언트가 맞은 효과를 보여주도록 시키기
        Client_OnHit(Damage);
    }
}

void ANetTestCharacter03_RPC::Server_Fire_Implementation()
{
    // 서버가 실행하는 코드

    if (ProjectileClass)
    {
        FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("FireSocket"));
        FRotator SpawnRotator = GetActorRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotator, SpawnParams);
    }
}

void ANetTestCharacter03_RPC::Client_OnHit_Implementation(float InDamage)
{
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->ClientStartCameraShake(CameraShakeClass);
    }

    //UNiagaraFunctionLibrary::SpawnSystemAtLocation(
    //    GetWorld(),
    //    HitVFX,
    //    GetActorLocation() + FVector::UpVector * 100.0f,
    //    FRotator::ZeroRotator,
    //    FVector::OneVector,
    //    true,
    //    true,
    //    ENCPoolMethod::AutoRelease);

    const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 120.f);

    ADamagePopupActor* Popup =
        GetWorld()->SpawnActor<ADamagePopupActor>(
            DamagePopupClass,
            SpawnLocation,
            FRotator::ZeroRotator
        );

    if (Popup)
    {
        Popup->Initialize(InDamage);
    }
}

void ANetTestCharacter03_RPC::Fire()
{
    // 내가 조종하고 있는 액터인지 확인
    if (IsLocallyControlled())
    {
        // 서버에게 발사 요청
        Server_Fire();
    }
}
