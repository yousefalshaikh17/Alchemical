// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Element.h"
#include "Interactable.h"
#include "ItemType.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "SeedBox.generated.h"

UCLASS()
class ALCHEMICAL_API ASeedBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASeedBox();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UInteractable* InteractableComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnPrimaryInteract(UInteractable* Interactable, AActor* ActorWhoTriggered, UInteractableController* ControllerThatTriggered);

	int GetPlantIndex() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GiveSeed(APlayerCharacter* PlayerCharacter);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeSeed(APlayerCharacter* PlayerCharacter);

	void UpdateInteractableState(const EItemType& CarriedItemType) const;

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
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EElement SeedType = EElement::Death;

};
