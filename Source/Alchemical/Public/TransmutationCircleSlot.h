// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemContainerComponent* ItemContainerComponent;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlaceIngredient(int32 NewPlantIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearIngredient();

	UFUNCTION(BlueprintPure)
	void GetIngredient(bool& bHasIngredient, int& PlantIndex) const;

	UFUNCTION(BlueprintPure)
	bool HasIngredient() const;
};
