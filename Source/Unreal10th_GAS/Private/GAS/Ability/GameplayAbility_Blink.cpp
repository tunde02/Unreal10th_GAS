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
        EndAbility(Handle, ActorInfo, ActivationInfo, true, /* bWasCancelled */ true);
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

bool UGameplayAbility_Blink::CheckCost(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    UGameplayEffect* CostGE = GetCostGameplayEffect();
    if (!CostGE) { return true; }

    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (!ASC) { return false; }

    if (!ASC->HasAttributeSetForAttribute(UStatAttributeSet::GetStaminaAttribute())
        || !ASC->HasAttributeSetForAttribute(UStatAttributeSet::GetStaminaCostAttribute()))
    {
        return false;
    }

    // 시전자의 현재 Stamina값 가져오기
    const float CurrentStamina = ASC->GetNumericAttribute(UStatAttributeSet::GetStaminaAttribute());

    // 필요 소모량을 알기 위해 Spec 생성 및 모디파이어 계산
    const FGameplayEffectContextHandle EffectContext = MakeEffectContext(Handle, ActorInfo);
    const float AbilityLevel = GetAbilityLevel(Handle, ActorInfo);
    FGameplayEffectSpec Spec(CostGE, EffectContext, AbilityLevel);
    Spec.CalculateModifierMagnitudes();

    // 현재 효과에서 StaminaCost 변경시키는 모디파이어를 전부 불러와서 적용 후 값 갱신
    float StaminaCost = 0.0f;
    bool bFoundStaminaModifier = false;
    for (int32 ModIndex = 0; ModIndex < Spec.Modifiers.Num(); ModIndex++)
    {
        if (Spec.Def && Spec.Def->Modifiers.IsValidIndex(ModIndex))
        {
            const FGameplayModifierInfo& ModDef = Spec.Def->Modifiers[ModIndex];
            if (ModDef.Attribute == UStatAttributeSet::GetStaminaCostAttribute())
            {
                const FModifierSpec& ModSpec = Spec.Modifiers[ModIndex];
                StaminaCost += ModSpec.GetEvaluatedMagnitude();
                bFoundStaminaModifier = true;
            }
        }
    }

    // 현재 효과에 StaminaCost 관련 모디파이어가 없으면 원래 함수 실행
    if (!bFoundStaminaModifier)
    {
        return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
    }

    if (StaminaCost <= CurrentStamina)
    {
        return true;
    }

    // 실패 원인인 코스트 부족 태그를 OptionalRelevantTags에 추가
    const FGameplayTag& CostTag = UAbilitySystemGlobals::Get().ActivateFailCostTag;
    if (OptionalRelevantTags && CostTag.IsValid())
    {
        OptionalRelevantTags->AddTag(CostTag);
    }

    return false;
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

    if (bTraceFloor)
    {
        // 공중에 떠있지 않게 하기
        FHitResult FloorHit;
        const FVector FloorStart = FinalDestination + FVector(0.0f, 0.0f, 50.0f);
        const FVector FloorEnd = FinalDestination - FVector(0.0f, 0.0f, TraceFloorDistance);
        if (World->LineTraceSingleByChannel(FloorHit, FloorStart, FloorEnd, ECC_Visibility, QueryParams))
        {
            FinalDestination.Z = FloorHit.ImpactPoint.Z + HalfHeight;
        }
    }

    return FinalDestination;
}
