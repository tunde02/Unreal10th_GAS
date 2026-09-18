// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_Blink.h"
#include "GAS/StatAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UGameplayAbility_Blink::UGameplayAbility_Blink()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Blink::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
    {
        // 캐릭터가 아닌 액터가 이 어빌리티를 사용함
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    const FVector StartLoc = Character->GetActorLocation();
    const FRotator CurrentRot = Character->GetActorRotation();

    const float CurrentLevel = GetAbilityLevel(Handle, ActorInfo);
    const float Distance = BlinkDistance.GetValueAtLevel(CurrentLevel);
    const FVector Destination = CalculateBlinkDestination(Character, Distance);

    const bool bTeleported = Character->TeleportTo(Destination, CurrentRot, false, true);
    if (!bTeleported)
    {
        Character->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, /* bWasCancelled */ false);
}

FVector UGameplayAbility_Blink::CalculateBlinkDestination(const ACharacter* InCharacter, float InDistance) const
{
    if (!InCharacter) { return FVector::ZeroVector; }

    UWorld* World = InCharacter->GetWorld();
    if (!World) { return InCharacter->GetActorLocation(); }

    const FVector StartLoc = InCharacter->GetActorLocation();
    const FVector DesiredDestination = StartLoc + (InCharacter->GetActorForwardVector() * InDistance);

    const UCapsuleComponent* Capsule = InCharacter->GetCapsuleComponent();
    const float Radius = Capsule->GetScaledCapsuleRadius();
    const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
    FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);

    const FVector SweepStart = StartLoc + FVector(0.0f, 0.0f, StepOffset);
    const FVector SweepEnd = DesiredDestination + FVector(0.0f, 0.0f, StepOffset);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(InCharacter);

    FHitResult SweepHit;
    bool bHit = World->SweepSingleByChannel(
        /* OutHit         */ SweepHit,
        /* Start          */ SweepStart,
        /* End            */ SweepEnd,
        /* Rot            */ FQuat::Identity,
        /* TraceChannel   */ ECollisionChannel::ECC_Visibility,
        /* CollisionShape */ CapsuleShape,
        /* Params         */ QueryParams);

    FVector FinalDestination = DesiredDestination;
    if (bHit
        && SweepHit.bBlockingHit
        && !SweepHit.bStartPenetrating
        && SweepHit.Time > 0.005f)
    {
        FinalDestination = SweepHit.Location + (SweepHit.ImpactNormal * CollisionOffset);
        FinalDestination.Z = StartLoc.Z;
    }

    //if (bTraceFloor)
    //{
    //    FHitResult FloorHit;
    //    const FVector FloorStart = FinalDestination + FVector(0.0f, 0.0f, 50.0f);
    //    const FVector FloorEnd = FinalDestination - FVector(0.0f, 0.0f, TraceFloorDistance);

    //    if (World->LineTraceSingleByChannel())
    //    {
    //        FinalDestination.Z = FloorHit.ImpactPoint.Z;
    //    }
    //}

    return FinalDestination;
}
