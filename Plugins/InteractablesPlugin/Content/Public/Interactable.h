// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "InteractableController.h"
#include "Interactable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInteract, UInteractable*, interactable, AActor*, actorWhoTriggered, UInteractableController*, controllerThatTriggered);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CGHERO_API UInteractable : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractable();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UShapeComponent* collider;

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
	virtual void Trigger(UInteractableController* controller);

	// Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FText ActionText;

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInteract OnInteract;
};
