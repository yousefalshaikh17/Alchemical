// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantData.h"
#include "ItemSlotActor.generated.h"

UCLASS()
class ALCHEMICAL_API AItemSlotActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSlotActor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSlotItem(const int32 NewPlantIndex);
	virtual void SetSlotItem_Implementation(const int32 NewPlantIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearSlot();
	virtual void ClearSlot_Implementation();

	UFUNCTION(BlueprintPure)
	virtual void GetPlantData(FPlantData& PlantData, bool& bFoundPlant) const;

	UFUNCTION(BlueprintPure)
	int GetSlotItem() const;

	UFUNCTION(BlueprintPure)
	bool IsSlotOccupied() const;
	
protected:
	int SlotPlantIndex = -1;

};
