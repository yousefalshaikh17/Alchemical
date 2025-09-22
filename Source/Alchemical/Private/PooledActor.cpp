// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActor.h"

// Sets default values
APooledActor::APooledActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APooledActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APooledActor::SetActiveState(bool bActive)
{
	SetActorEnableCollision(bActive);
	SetActorHiddenInGame(!bActive);
	SetActorTickEnabled(bActive);
}

void APooledActor::ActivateFromPool()
{
	SetActiveState(true);

	if (bIsActive) return;

	bIsActive = true;
	PooledBeginPlay();
}

void APooledActor::DeactivateFromPool()
{
	SetActiveState(false);

	if (!bIsActive) return;

	bIsActive = false;
	PooledEndPlay();
}

void APooledActor::Deactivate()
{
	DeactivateFromPool();
}

