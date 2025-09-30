// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"
#include "PooledActor.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
}


void UActorPool::InitializePool()
{
	if (!PoolActorClass) return;

	for (int i = 0; i < PoolSize; i++)
	{
		CreatePoolActor();
	}
}

APooledActor* UActorPool::CreatePoolActor()
{
	APooledActor* Actor = GetWorld()->SpawnActor<APooledActor>(PoolActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (!Actor) return nullptr;

	Actor->DeactivateFromPool();

	Pool.Add(Actor);

	return Actor;
}

APooledActor* UActorPool::RequestActor(const FTransform& SpawnTransform)
{
	APooledActor* PoolActor = nullptr;

	for (const auto& Actor : Pool)
	{
		if (!IsValid(Actor) || Actor->bIsActive) continue;

		PoolActor = Actor;
		break;
	}

	if (!PoolActor && bIsDynamicallySized)
	{
		PoolActor = CreatePoolActor();
		PoolSize++;
	}

	if (PoolActor)
	{
		PoolActor->SetActorTransform(SpawnTransform);
		PoolActor->ActivateFromPool();
	}

	return PoolActor;
}
