// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

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
	collider = Cast<UShapeComponent>(GetAttachParent());
	if (!collider)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Attach Collider: %s"), *GetFName().ToString());
		DestroyComponent();
		return;
	}
	
	// Bind collider events
	collider->OnComponentBeginOverlap.AddDynamic(this, &UInteractable::OnColliderBeginOverlap);
	collider->OnComponentEndOverlap.AddDynamic(this, &UInteractable::OnColliderEndOverlap);
}

// Handle when collider is overlapping with a character or actor with UInteractableController
void UInteractable::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UInteractableController* controller = OtherActor->GetComponentByClass<UInteractableController>();
	if (!controller) return;

	controller->RegisterInteractable(this);
}

// Handle when collider is no longer overlapping with a character or actor with UInteractableController
void UInteractable::OnColliderEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UInteractableController* controller = OtherActor->GetComponentByClass<UInteractableController>();
	if (!controller) return;

	controller->UnregisterInteractable(this);
}

void UInteractable::UpdateDisplay()
{
	if (!DisplayingController) return;

	DisplayingController->UpdateWidgetDisplay();
}

// Triggers the interactable which fires the OnInteract event.
void UInteractable::TriggerPrimary(UInteractableController* controller)
{
	if (!PrimaryActionEnabled) return;

	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot trigger Interactable without a controller."));
		return;
	}

	AActor* actor = controller->GetOwner();
	OnPrimaryInteract.Broadcast(this, actor, controller);
}

void UInteractable::TriggerSecondary(UInteractableController* controller)
{
	if (!SecondaryActionEnabled) return;

	if (!controller)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot trigger Interactable without a controller."));
		return;
	}

	AActor* actor = controller->GetOwner();
	OnSecondaryInteract.Broadcast(this, actor, controller);
}

