// Fill out your copyright notice in the Description page of Project Settings.


#include "TransmutationCircleSlot.h"

// Sets default values
ATransmutationCircleSlot::ATransmutationCircleSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATransmutationCircleSlot::K2_OnReset()
{
	ClearIngredient();
}

void ATransmutationCircleSlot::GetIngredient(bool& bHasIngredient, int& PlantIndex) const
{
	bHasIngredient = false;
	
	if (!ItemContainerComponent) return;

	FItemInstance Item;
	ItemContainerComponent->GetItem(Item);

	bHasIngredient = Item.Type == EItemType::Plant;
	PlantIndex = Item.PlantIndex;
}

bool ATransmutationCircleSlot::HasIngredient() const
{
	if (!ItemContainerComponent) return false;
	return ItemContainerComponent->HasItem();
}

void ATransmutationCircleSlot::ClearIngredient_Implementation()
{
	if (!ItemContainerComponent) return;

	ItemContainerComponent->ClearItem();
}

void ATransmutationCircleSlot::PlaceIngredient_Implementation(const int32 NewPlantIndex)
{
	if (!ItemContainerComponent) return;
	
	const FItemInstance NewItem = {
		EItemType::Plant,
		NewPlantIndex
	};
	ItemContainerComponent->SetItem(NewItem);
}
