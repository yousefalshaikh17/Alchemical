// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemContainerComponent.h"

#include "GameDataSubsystem.h"

// Sets default values for this component's properties
UItemContainerComponent::UItemContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

bool UItemContainerComponent::SetItem_Implementation(const FItemInstance& NewItem)
{
	if (IsSameCarriedItem(NewItem))
		return false;
	
	CurrentItem = NewItem;

	if (NewItem.Type == EItemType::None)
		CurrentItem.PlantIndex = -1;

	// Update display
	UpdateItemDisplay();

	// Broadcast event
	OnItemChanged.Broadcast(this, CurrentItem);

	return true;
}


void UItemContainerComponent::GetItemPlantData(bool& bFoundPlant, FPlantData& PlantData) const
{
	bFoundPlant = false;
	
	if (!HasItem()) return;
	
	const UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!GameDataSubsystem) return;

	GameDataSubsystem->GetPlantData(CurrentItem.PlantIndex, PlantData, bFoundPlant);
}

void UItemContainerComponent::GetItem(FItemInstance& Item) const
{
	Item = CurrentItem;
}

bool UItemContainerComponent::HasItem() const
{
	return CurrentItem.Type != EItemType::None;
}

bool UItemContainerComponent::IsSameCarriedItem(const FItemInstance& Item) const
{
	return CurrentItem.Type == Item.Type &&
		(CurrentItem.Type == EItemType::None || CurrentItem.PlantIndex == Item.PlantIndex);
}

void UItemContainerComponent::UpdateItemDisplay() const
{
	if (CurrentItem.Type == EItemType::None)
	{
		if (ElementsDisplayWidget)
			ElementsDisplayWidget->ClearElementDisplay();

		if (PlantSpriteComponent)
			PlantSpriteComponent->SetVisibility(false);
		return;
	}

	FPlantData PlantData;
	bool bFound = false;
	GetItemPlantData(bFound, PlantData);

	if (bFound)
	{
		UPaperSprite* NewSprite;
		switch (CurrentItem.Type)
		{
		case EItemType::Seed:
			NewSprite = SeedSprite;
			break;
		case EItemType::Plant:
			NewSprite = PlantData.Sprite;
			break;
		default:
			NewSprite = nullptr;
		}

		if (ElementsDisplayWidget)
			ElementsDisplayWidget->UpdateElementDisplay(PlantData.Elements);

		if (PlantSpriteComponent)
		{
			PlantSpriteComponent->SetSprite(NewSprite);
			PlantSpriteComponent->SetVisibility(true);	
		}
	}
}

void UItemContainerComponent::ClearItem_Implementation()
{
	SetItem(FItemInstance{});
}
