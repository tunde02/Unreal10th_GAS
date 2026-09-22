// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Test04/Test04.h"
#include "Test/TestCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

void ATest04::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    DrawDebugRadius();
}

void ATest04::TestScanAndHit()
{
    UWorld* World = GetWorld();
    if (!World) { return; }

    DrawDebugRadius();

    const FVector Origin = GetActorLocation();

    // 캡처할 타입 지정
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    // 캡쳐 안할 오브젝트 지정
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(this);
    if (GetInstigator())
    {
        IgnoreActors.Add(GetInstigator());
    }

    // 오버랩으로 후보들 모집
    TArray<AActor*> OverlappingActors;
    UKismetSystemLibrary::SphereOverlapActors(
        World,
        Origin,
        Radius,
        ObjectTypes,
        AActor::StaticClass(),
        IgnoreActors,
        OverlappingActors
    );

    FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(ATest04_LoSTrace), false, this);
    TraceParams.AddIgnoredActor(this);
    if (GetInstigator())
    {
        TraceParams.AddIgnoredActor(GetInstigator());
    }

    for (AActor* Overlapped : OverlappingActors)
    {
        if (!IsValid(Overlapped)) { continue; }

        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Overlapped);
        if (!TargetASC) { continue; }

        FVector TargetLocation = Overlapped->GetActorLocation();

        FHitResult HitResult;
        const bool bHit = World->LineTraceSingleByChannel(
            HitResult,
            Origin,
            TargetLocation,
            ECC_Camera,
            TraceParams
        );

        // 목표로 한 대상과 라인트레이스 성공
        const bool bConfirmed = bHit && (HitResult.GetActor() == Overlapped);
        const FVector TraceEnd = bHit ? HitResult.ImpactPoint : TargetLocation;

        DrawDebugLine(World, Origin, TraceEnd, bConfirmed ? FColor::Green : FColor::Red, false, 1.0f, 0, 2.0f);

        if (bConfirmed && HitEffectClass)
        {
            FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
            EffectContextHandle.AddSourceObject(this);
            EffectContextHandle.AddInstigator(this, this);
            EffectContextHandle.AddHitResult(HitResult, true); // HitResult를 넣어줘야 큐를 생성할 위치를 알 수 있음

            FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(HitEffectClass, EffectLevel, EffectContextHandle);
            if (SpecHandle.IsValid())
            {
                TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }
    }
}

void ATest04::TestDebuff()
{
    if (!Target) { return; }
    if (!DebuffEffectClass) { return; }

    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
    if (!TargetASC) { return; }

    FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(this);
    EffectContextHandle.AddInstigator(this, this);

    FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DebuffEffectClass, EffectLevel, EffectContextHandle);
    if (SpecHandle.IsValid())
    {
        TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
}

void ATest04::DrawDebugRadius()
{
    if (UWorld* World = GetWorld())
    {
        FlushPersistentDebugLines(World);

        DrawDebugSphere(
            World, GetActorLocation(), Radius, 32, FColor::Cyan, true, -1.0f, 0, 1.5f);
    }
}
