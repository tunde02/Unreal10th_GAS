// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Test03/TestPlayerCharacter03.h"

#include "AbilitySystemComponent.h"
#include "UObject/ConstructorHelpers.h"

ATestPlayerCharacter03::ATestPlayerCharacter03()
{
    static ConstructorHelpers::FClassFinder<UGameplayAbility> SprintAbility(
        TEXT("/Game/Blueprints/GAS/Ability/Sprint/GA_Sprint"));
    static ConstructorHelpers::FClassFinder<UGameplayAbility> JumpAbility(
        TEXT("/Game/Blueprints/GAS/Ability/SuperJump/GA_SuperJump"));

    if (SprintAbility.Succeeded())
    {
        FDefaultAbilityEntry& Entry = DefaultAbilities.AddDefaulted_GetRef();
        Entry.AbilityClass = SprintAbility.Class;
        Entry.Input = EDefaultAbilityInput::Sprint;
    }

    if (JumpAbility.Succeeded())
    {
        FDefaultAbilityEntry& Entry = DefaultAbilities.AddDefaulted_GetRef();
        Entry.AbilityClass = JumpAbility.Class;
        Entry.Input = EDefaultAbilityInput::Jump;
    }
}

void ATestPlayerCharacter03::GiveTestAbility()
{
    if (!TestAbilityClass) { return; }

    // 이미 부여된 어빌리티를 다시 부여하는 상황
    if (FGameplayAbilitySpec* ExistingSpec = AbilitySystemComponent->FindAbilitySpecFromClass(TestAbilityClass))
    {
        ExistingSpec->Level = TestAbilityLevel;
        AbilitySystemComponent->MarkAbilitySpecDirty(*ExistingSpec); // 어빌리티 스펙 정보가 바꼈다고 알림
        return;
    }

    // 새로운 어빌리티를 부여하는 상황
    FGameplayAbilitySpec Spec(TestAbilityClass, TestAbilityLevel);
    FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);

    if (Handle.IsValid()) { /* 어빌리티 부여 성공 */ }
    else { /* 어빌리티 부여 실패 */ }
}

void ATestPlayerCharacter03::ClearTestAbility()
{
    if (!TestAbilityClass) { return; }

    if (FGameplayAbilitySpec* ExistingSpec = AbilitySystemComponent->FindAbilitySpecFromClass(TestAbilityClass))
    {
        AbilitySystemComponent->ClearAbility(ExistingSpec->Handle);
    }
}

void ATestPlayerCharacter03::ClearAllTestAbility()
{
    AbilitySystemComponent->ClearAllAbilities();
}

void ATestPlayerCharacter03::ActivateTestAbility()
{
    if (!TestAbilityClass) { return; }

    const bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(TestAbilityClass);

    if (bSuccess) { /* 어빌리티 발동 성공 */ }
    else { /* 어빌리티 발동 실패 (쿨타임, 코스트 등) */ }
}

void ATestPlayerCharacter03::DeactivateTestAbility()
{
    if (!TestAbilityClass) { return; }

    if (FGameplayAbilitySpec* ExistingSpec = AbilitySystemComponent->FindAbilitySpecFromClass(TestAbilityClass);
        ExistingSpec
        && ExistingSpec->IsActive())
    {
        AbilitySystemComponent->CancelAbilityHandle(ExistingSpec->Handle);
    }
}

void ATestPlayerCharacter03::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

void ATestPlayerCharacter03::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
}
