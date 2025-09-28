// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"
#include "ElementTypesDisplayWidget.h"
#include "ItemInstance.h"
#include "PaperSpriteComponent.h"
#include "PlantData.h"
#include "Components/ActorComponent.h"
#include "ItemContainerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, UItemContainerComponent*, Container, FItemInstance, Item);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class ALCHEMICAL_API UItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemContainerComponent();

	// Returns true if successful.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool SetItem(const FItemInstance& NewItem);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void ClearItem();

    UFUNCTION(BlueprintPure)
    virtual void GetItemPlantData(bool& bFoundPlant, FPlantData& PlantData) const;

    UFUNCTION(BlueprintPure)
    void GetItem(FItemInstance& Item) const;

    UFUNCTION(BlueprintPure)
    bool HasItem() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnItemChanged OnItemChanged;
	
protected:	
	FItemInstance CurrentItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* SeedSprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UElementTypesDisplayWidget* ElementsDisplayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* PlantSpriteComponent;

	bool IsSameCarriedItem(const FItemInstance& Item) const;
	void UpdateItemDisplay() const;
		
};
