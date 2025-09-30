// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemicalGameMode.h"

void AAlchemicalGameMode::RegisterResettableActor_Implementation(AActor* Actor)
{
	ResettableActors.Add(Actor);
}

void AAlchemicalGameMode::UnregisterResettableActor_Implementation(const AActor* Actor)
{
	ResettableActors.Remove(Actor);
}

void AAlchemicalGameMode::ResetLevel()
{
	for (const auto WeakActor : ResettableActors)
	{
		if (AActor* Actor = WeakActor.Get())
			Actor->Reset();
	}
}
