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
	APooledActor* actor = GetWorld()->SpawnActor<APooledActor>(PoolActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (!actor) return nullptr;

	actor->DeactivateFromPool();

	Pool.Add(actor);

	return actor;
}

APooledActor* UActorPool::RequestActor(const FTransform& SpawnTransform)
{
	APooledActor* poolActor = nullptr;

	for (auto& actor : Pool)
	{
		if (!IsValid(actor) || actor->bIsActive) continue;

		poolActor = actor;
		break;
	}

	if (!poolActor && bIsDynamicallySized)
	{
		poolActor = CreatePoolActor();
		PoolSize++;
	}

	if (poolActor)
	{
		poolActor->SetActorTransform(SpawnTransform);
		poolActor->ActivateFromPool();
	}

	return poolActor;
}
