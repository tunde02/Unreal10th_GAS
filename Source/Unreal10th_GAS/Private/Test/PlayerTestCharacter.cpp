// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/PlayerTestCharacter.h"
#include "GAS/StatAttributeSet.h"

void APlayerTestCharacter::ModifyHealth(float InAmount)
{
    if (!StatAttributeSet)
    {
        UE_LOG(LogTemp, Warning, TEXT("[APlayerTestCharacter::ModifyHealth()] : StatAttributeSet가 nullptr입니다."));
        return;
    }

    const float NewHealth = StatAttributeSet->GetHealth() + InAmount;
    StatAttributeSet->SetHealth(NewHealth);
}

void APlayerTestCharacter::ModifyStamina(float InAmount)
{
    if (!StatAttributeSet)
    {
        UE_LOG(LogTemp, Warning, TEXT("[APlayerTestCharacter::ModifyStamina()] : StatAttributeSet가 nullptr입니다."));
        return;
    }

    const float NewStamina = StatAttributeSet->GetStamina() + InAmount;
    StatAttributeSet->SetStamina(NewStamina);
}
