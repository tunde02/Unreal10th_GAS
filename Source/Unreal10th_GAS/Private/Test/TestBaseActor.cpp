// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestBaseActor.h"

// Sets default values
ATestBaseActor::ATestBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

