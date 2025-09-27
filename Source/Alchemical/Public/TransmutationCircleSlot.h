// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSlotActor.h"
#include "PaperSpriteComponent.h"
#include "ElementTypesDisplayWidget.h"
#include "TransmutationCircleSlot.generated.h"

UCLASS()
class ALCHEMICAL_API ATransmutationCircleSlot : public AItemSlotActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransmutationCircleSlot();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* PlantSpriteComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UElementTypesDisplayWidget* ElementsDisplayWidget;
	
	void UpdatePlantDisplay() const;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlaceIngredient(int32 NewPlantIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearIngredient();
};
