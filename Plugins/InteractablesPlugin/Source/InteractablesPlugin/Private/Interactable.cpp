// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "Components/ShapeComponent.h"

// Sets default values for this component's properties
UInteractable::UInteractable()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInteractable::BeginPlay()
{
	Super::BeginPlay();

	// Initialize collider variable
	Collider = Cast<UShapeComponent>(GetAttachParent());
	if (!Collider)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Attach Collider: %s"), *GetFName().ToString());
		DestroyComponent();
		return;
	}
	
	// Bind collider events
	Collider->OnComponentBeginOverlap.AddDynamic(this, &UInteractable::OnColliderBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &UInteractable::OnColliderEndOverlap);
}

// Handle when collider is overlapping with a character or actor with UInteractableController
void UInteractable::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UInteractableController* Controller = OtherActor->GetComponentByClass<UInteractableController>();
	if (!Controller) return;

	Controller->RegisterInteractable(this);
}

// Handle when collider is no longer overlapping with a character or actor with UInteractableController
void UInteractable::OnColliderEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UInteractableController* Controller = OtherActor->GetComponentByClass<UInteractableController>();
	if (!Controller) return;

	Controller->UnregisterInteractable(this);
}

void UInteractable::UpdateDisplay()
{
	if (!DisplayingController) return;

	DisplayingController->UpdateWidgetDisplay();
}

// Triggers the interactable which fires the OnInteract event.
void UInteractable::TriggerPrimary(UInteractableController* Controller)
{
	if (!PrimaryActionEnabled) return;

	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot trigger Interactable without a controller."));
		return;
	}

	AActor* Actor = Controller->GetOwner();
	OnPrimaryInteract.Broadcast(this, Actor, Controller);
}

void UInteractable::TriggerSecondary(UInteractableController* Controller)
{
	if (!SecondaryActionEnabled) return;

	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot trigger Interactable without a controller."));
		return;
	}

	AActor* Actor = Controller->GetOwner();
	OnSecondaryInteract.Broadcast(this, Actor, Controller);
}

