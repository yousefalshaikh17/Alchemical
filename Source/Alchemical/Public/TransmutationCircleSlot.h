// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransmutationCircleSlot.generated.h"

UCLASS()
class ALCHEMICAL_API ATransmutationCircleSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransmutationCircleSlot();
protected:
	int PlantIndex = -1;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlaceIngredient(int32 NewPlantIndex);
	virtual void PlaceIngredient_Implementation(int32 NewPlantIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearIngredient();
	virtual void ClearIngredient_Implementation();

	UFUNCTION(BlueprintPure)
	int GetCurrentIngredient() const;

	UFUNCTION(BlueprintPure)
	bool IsOccupied() const;
};
