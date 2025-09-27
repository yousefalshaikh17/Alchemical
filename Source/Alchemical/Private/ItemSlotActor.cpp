// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotActor.h"

#include "GameDataSubsystem.h"

// Sets default values
AItemSlotActor::AItemSlotActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItemSlotActor::SetSlotItem_Implementation(const int32 NewPlantIndex)
{
	if (NewPlantIndex < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Plant index cannot be negative."));
		return;
	}

	SlotPlantIndex = NewPlantIndex;
}

void AItemSlotActor::ClearSlot_Implementation()
{
	SlotPlantIndex = -1;
}

void AItemSlotActor::GetPlantData(FPlantData& PlantData, bool& bFoundPlant) const
{
	bFoundPlant = false;
	
	if (!IsSlotOccupied()) return;
	
	const UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!GameDataSubsystem) return;

	GameDataSubsystem->GetPlantData(SlotPlantIndex, PlantData, bFoundPlant);
}

int AItemSlotActor::GetSlotItem() const
{
	return SlotPlantIndex;
}

bool AItemSlotActor::IsSlotOccupied() const
{
	return SlotPlantIndex > -1;
}
