// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"
#include "ElementTypesDisplayWidget.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PaperSpriteComponent.h"
#include "PlantData.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHeldItemChanged, APlayerCharacter*, Character, EItemType, CarriedItemType, int, PlantIndex);

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
	UElementTypesDisplayWidget* ElementsDisplayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* PlantSpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* SeedSprite;

	

	// Called when the game starts or when spawned
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Move(float RightScale, float ForwardScale);

    UFUNCTION(BlueprintNativeEvent)
	void HandleMoveInput(const FInputActionValue& Value);
	
	bool GetHeldPlantData(FPlantData& PlantData) const;
	void UpdateHeldItemDisplay() const;

	bool IsSameCarriedItem(EItemType NewCarriedItemType, int NewPlantIndex) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMovementEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType CarriedItemType = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlantIndex = -1;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateHeldItem(EItemType NewCarriedItemType, int NewPlantIndex);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHeldItemChanged OnHeldItemChanged;

};
