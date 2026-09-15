// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/EnemyTestCharacter.h"
#include "Widget/HealthAndStaminaWidget.h"
#include "GAS/StatAttributeSet.h"

#include "Components/WidgetComponent.h"
#include "AbilitySystemComponent.h"

AEnemyTestCharacter::AEnemyTestCharacter()
{
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatBarWidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);
}

void AEnemyTestCharacter::ModifyHealth(float InAmount)
{
	if (!StatAttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEnemyTestCharacter::ModifyHealth()] : StatAttributeSet가 nullptr입니다."));
		return;
	}

	const float NewHealth = StatAttributeSet->GetHealth() + InAmount;
	StatAttributeSet->SetHealth(NewHealth);
}

void AEnemyTestCharacter::ModifyStamina(float InAmount)
{
	if (!StatAttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEnemyTestCharacter::ModifyStamina()] : StatAttributeSet가 nullptr입니다."));
		return;
	}

	const float NewStamina = StatAttributeSet->GetStamina() + InAmount;
	StatAttributeSet->SetStamina(NewStamina);
}

void AEnemyTestCharacter::BeginPlay()
{
    Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		Cast<UHealthAndStaminaWidget>(WidgetComponent->GetWidget())->BindToAttribute(
			AbilitySystemComponent,
			UStatAttributeSet::GetHealthAttribute(),
			UStatAttributeSet::GetMaxHealthAttribute(),
			UStatAttributeSet::GetStaminaAttribute(),
			UStatAttributeSet::GetMaxStaminaAttribute());

		//FOnGameplayAttributeValueChange& HealthChange = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetHealthAttribute());
		//HealthChange.AddUObject(this, &ATestCharacter::OnHealthChanged);
	}
}
