// Fill out your copyright notice in the Description page of Project Settings.


#include "TransmutationCircleSlot.h"

// Sets default values
ATransmutationCircleSlot::ATransmutationCircleSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATransmutationCircleSlot::UpdatePlantDisplay() const
{
	if (!PlantSpriteComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PlantSpriteComponent is null!"));
		return;
	}

	if (!ElementsDisplayWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ElementsDisplayWidget is null!"));
		return;
	}
	
	FPlantData PlantData;
	bool bFound = false;
	GetPlantData(PlantData, bFound);
	
	if (bFound)
	{
		PlantSpriteComponent->SetSprite(PlantData.Sprite);
		ElementsDisplayWidget->UpdateElementDisplay(PlantData.Elements);
	} else
	{
		ElementsDisplayWidget->ClearElementDisplay();
	}

	PlantSpriteComponent->SetVisibility(bFound);
}

void ATransmutationCircleSlot::ClearIngredient_Implementation()
{
	ClearSlot();
	UpdatePlantDisplay();
}

void ATransmutationCircleSlot::PlaceIngredient_Implementation(int32 NewPlantIndex)
{
	SetSlotItem(NewPlantIndex);
	UpdatePlantDisplay();
}
