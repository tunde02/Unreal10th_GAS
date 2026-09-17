// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Test02/TestTrapActor.h"
#include "GAS/StatAttributeSet.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"

ATestTrapActor::ATestTrapActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetupAttachment(RootComponent);

    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
    Stat = CreateDefaultSubobject<UStatAttributeSet>(TEXT("Stat"));
}

void ATestTrapActor::BeginPlay()
{
    Super::BeginPlay();
}

void ATestTrapActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATestTrapActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    ApplyGameplayEffect(OtherActor);
}

void ATestTrapActor::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    RemoveGameplayEffect(OtherActor);
}

void ATestTrapActor::ApplyGameplayEffect(AActor* Target)
{
    if (!GameplayEffectClass) { return; }
    if (!ASC) { return; }
    if (!Stat) { return; }
    if (!Target) { return; }

    IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target);
    if (!ASI) { return; }

    UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent();
    if (!TargetASC) return;

    FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);
    EffectContext.AddInstigator(this, this);

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContext);
    if (!SpecHandle.IsValid()) { return; }

    FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

void ATestTrapActor::RemoveGameplayEffect(AActor* Target)
{
    if (!GameplayEffectClass) { return; }
    if (!ASC) { return; }
    if (!Stat) { return; }
    if (!Target) { return; }

    IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target);
    if (!ASI) { return; }

    UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent();
    if (!TargetASC) return;

    TargetASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, ASC);
}

