// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "InteractableInterface.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InteractableController.generated.h"

class UInteractable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CGHERO_API UInteractableController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableController();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TMap<ECommonInputType, FText> InputDisplayTextMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractInputAction;


	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* InteractableWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (MustImplement = "UInteractableInterface"))
	TSubclassOf<UUserWidget> InteractableWidgetClass;

	TSet<UInteractable*> ActiveInteractables;
	UInteractable* BestInteractable;

	ECommonInputType ActiveInputType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	FText UnknownInputDisplayText;

	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every tick
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual UInteractable* GetClosestInteractable();
	virtual void UpdateBestInteractable();
	virtual FText GetInputDisplayText(ECommonInputType inputType);
	virtual void UpdateWidgetDisplay();

	// Event
	virtual void OnInputChanged(ECommonInputType newInputType);

	UFUNCTION()
	void OnInteractPressed();

public:	
	virtual void RegisterInteractable(UInteractable* interactable);
	virtual void UnregisterInteractable(UInteractable* interactable);
};
