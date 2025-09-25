// Fill out your copyright notice in the Description page of Project Settings.


#include "TransmutationCircleSlot.h"

// Sets default values
ATransmutationCircleSlot::ATransmutationCircleSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATransmutationCircleSlot::PlaceIngredient_Implementation(int32 NewPlantIndex)
{
	if (NewPlantIndex < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Plant index cannot be negative."));
		return;
	}

	this->PlantIndex = NewPlantIndex;
}

void ATransmutationCircleSlot::ClearIngredient_Implementation()
{
	this->PlantIndex = -1;
}

int ATransmutationCircleSlot::GetCurrentIngredient() const
{
	return PlantIndex;
}

bool ATransmutationCircleSlot::IsOccupied() const
{
	return PlantIndex > -1;
}
