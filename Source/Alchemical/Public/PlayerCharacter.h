// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"
#include "ElementTypesDisplayWidget.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ItemContainerComponent.h"
#include "ItemInstance.h"
#include "PaperSpriteComponent.h"
#include "PlantData.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeldItemChanged, APlayerCharacter*, Character, FItemInstance, Item);

UCLASS()
class ALCHEMICAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* CameraTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* SeedSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemContainerComponent* ItemContainerComponent;

	

	// Called when the game starts or when spawned
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Move(float RightScale, float ForwardScale);

    UFUNCTION(BlueprintNativeEvent)
	void HandleMoveInput(const FInputActionValue& Value);

	UFUNCTION(BlueprintPure)
	void GetHeldPlantData(bool& bFoundPlant, FPlantData& PlantData) const;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMovementEnabled = true;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateHeldItem(const FItemInstance NewItem);

	UFUNCTION(BlueprintPure)
	void GetHeldItem(FItemInstance& Item) const;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHeldItemChanged OnHeldItemChanged;

	/**
	 * Override to prevent destruction on level reset. Uses AActor implementation.
	 */
	virtual void Reset() override { AActor::Reset(); }
};
