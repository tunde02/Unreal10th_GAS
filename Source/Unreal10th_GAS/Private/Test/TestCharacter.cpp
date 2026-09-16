// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCharacter.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"

ATestCharacter::ATestCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
    StatAttributeSet = CreateDefaultSubobject<UStatAttributeSet>(TEXT("Stat"));
}

UAbilitySystemComponent* ATestCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UStatAttributeSet* ATestCharacter::GetStatAttribute() const
{
    return StatAttributeSet;
}

void ATestCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ATestCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATestCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (IsValid(AbilitySystemComponent))
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);

        //FOnGameplayAttributeValueChange& HealthChange = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetHealthAttribute());
        //HealthChange.AddUObject(this, &ATestCharacter::OnHealthChanged);
    }
}

//void ATestCharacter::OnHealthChanged(const FOnAttributeChangeData& InData)
//{
//    UE_LOG(LogTemp, Log, TEXT("[ATestCharacter::OnHealthChanged()] : Health 변경됨 (%.1f) -> (%.1f)"),
//           InData.OldValue, InData.NewValue);
//}
//
//void ATestCharacter::OnStaminaChanged(const FOnAttributeChangeData& InData)
//{
//    UE_LOG(LogTemp, Log, TEXT("[ATestCharacter::OnStaminaChanged()] : Stamina 변경됨 (%.1f) -> (%.1f)"),
//           InData.OldValue, InData.NewValue);
//}
