// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "InteractableController.h"
#include "Interactable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInteract, UInteractable*, Interactable, AActor*, ActorWhoTriggered, UInteractableController*, ControllerThatTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPromptDisplay, UInteractable*, Interactable, AActor*, ControllerOwner, UInteractableController*, ControllerComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTABLESPLUGIN_API UInteractable : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractable();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UShapeComponent* Collider;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Events
	UFUNCTION()
	void OnColliderBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnColliderEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);


public:
	UFUNCTION()
	virtual void TriggerPrimary(UInteractableController* Controller);
	UFUNCTION()
	virtual void TriggerSecondary(UInteractableController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Display")
	virtual void UpdateDisplay();

	// Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FText PrimaryActionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FText SecondaryActionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool PrimaryActionEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool SecondaryActionEnabled = false;

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInteract OnPrimaryInteract;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInteract OnSecondaryInteract;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPromptDisplay OnPromptShown;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPromptDisplay OnPromptHidden;

	// State
	UInteractableController* DisplayingController;
};
