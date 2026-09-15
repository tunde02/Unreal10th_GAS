// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainWidget.h"
#include "Widget/StatBarWidget.h"
#include "Test/TestCharacter.h"
#include "GAS/StatAttributeSet.h"

#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

void UMainWidget::BindToCharacter(ACharacter* InCharacter)
{
    if (!InCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("[UMainWidget::BindToCharacter()] : InCharacter가 nullptr입니다."));
        return;
    }

    ATestCharacter* Character = Cast<ATestCharacter>(InCharacter);
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("[UMainWidget::BindToCharacter()] : InCharacter를 ATestCharacter로 형변환 할 수 없습니다."));
        return;
    }

    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[UMainWidget::BindToCharacter()] : InCharacter의 UAbilitySystemComponent가 nullptr입니다."));
        return;
    }

    HealthBarWidget->BindToAttribute(
        ASC,
        UStatAttributeSet::GetHealthAttribute(),
        UStatAttributeSet::GetMaxHealthAttribute());
    
    StaminaBarWidget->BindToAttribute(
        ASC,
        UStatAttributeSet::GetStaminaAttribute(),
        UStatAttributeSet::GetMaxStaminaAttribute());

    //FOnGameplayAttributeValueChange& HealthChange =
    //    Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetStatAttribute()->GetHealthAttribute());
    //HealthChange.AddUObject(HealthBarWidget, &UStatBarWidget::RefreshStatBar);

    //UE_LOG(LogTemp, Warning,
    //       TEXT("HealthBar=%s, Handle=%s"),
    //       *GetNameSafe(HealthBarWidget),
    //       Handle.IsValid() ? TEXT("Valid") : TEXT("Invalid"));

    //FOnGameplayAttributeValueChange& StaminaChange =
    //    Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetStatAttribute()->GetStaminaAttribute());
    //StaminaChange.AddUObject(StaminaBarWidget, &UStatBarWidget::RefreshStatBar);
}
