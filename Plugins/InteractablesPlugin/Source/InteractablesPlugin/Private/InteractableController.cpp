// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableController.h"
#include "Interactable.h"


// Sets default values for this component's properties
UInteractableController::UInteractableController()
{
	PrimaryComponentTick.bCanEverTick = true;
	UnknownInputDisplayText = FText::FromString("UNKNOWN");
}


// Called when the game starts
void UInteractableController::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UCommonInputSubsystem* commonInputSubsystem = UCommonInputSubsystem::Get(playerController->GetLocalPlayer());
	// Bind input changed
	commonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &UInteractableController::OnInputChanged);
	// Initialize input type
	ActiveInputType = commonInputSubsystem->GetCurrentInputType();

	// Set up player input
	AActor* owner = GetOwner();
	if (UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(owner->InputComponent))
	{
		enhancedInput->BindAction(PrimaryInteractInputAction, ETriggerEvent::Started, this, &UInteractableController::OnPrimaryInteractPressed);
		enhancedInput->BindAction(SecondaryInteractInputAction, ETriggerEvent::Started, this, &UInteractableController::OnSecondaryInteractPressed);
	}

	// Create UI
	if (InteractableWidgetClass)
	{
		if (InteractableWidgetClass->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			InteractableWidget = CreateWidget<UUserWidget>(GetWorld(), InteractableWidgetClass);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Widget does not inherit InteractableInterface: %s"), *GetFName().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget not provided: %s"), *GetFName().ToString());
	}

	UpdateBestInteractable();
}

// Update best interactable every tick
void UInteractableController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UpdateBestInteractable();
}

// Returns the closest interactable to the owning actor
UInteractable* UInteractableController::GetClosestInteractable()
{
	if (ActiveInteractables.IsEmpty())
		return nullptr;

	float minDistanceSquared = INFINITY;
	UInteractable* bestCandidate = nullptr;

	FVector actorLocation = GetOwner()->GetActorLocation();

	for (auto& interactable : ActiveInteractables)
	{
		if (!interactable->PrimaryActionEnabled && !interactable->SecondaryActionEnabled) continue;

		// If there is no candidate, initialize with the current candidate.
		if (!bestCandidate)
		{
			bestCandidate = interactable;
			continue;
		}

		// Get distance squared. Distance squared saves computation time as it avoids the sqrt() operation.
		FVector location = interactable->GetComponentLocation();
		float distanceSquared = FVector::DistSquared(actorLocation, location);

		// If distance is less than the current minimum, update it.
		if (distanceSquared < minDistanceSquared)
		{
			minDistanceSquared = distanceSquared;
			bestCandidate = interactable;
		}
	}

	return bestCandidate;
}

// Updates best interactable variable and display
void UInteractableController::UpdateBestInteractable()
{
	UInteractable* newBest = GetClosestInteractable();
	if (newBest == BestInteractable)
		return;

	if (BestInteractable)
		BestInteractable->DisplayingController = nullptr;

	if (newBest)
		newBest->DisplayingController = this;

	BestInteractable = newBest;
	UpdateWidgetDisplay();
} 

// Given an input type, returns the user-facing display text
FText UInteractableController::GetPrimaryInputDisplayText(ECommonInputType inputType)
{
	bool hasDisplayText = PrimaryInputDisplayTextMap.Contains(inputType);
	if (!hasDisplayText) return UnknownInputDisplayText;

	return PrimaryInputDisplayTextMap[inputType];
}

FText UInteractableController::GetSecondaryInputDisplayText(ECommonInputType inputType)
{
	bool hasDisplayText = SecondaryInputDisplayTextMap.Contains(inputType);
	if (!hasDisplayText) return UnknownInputDisplayText;

	return SecondaryInputDisplayTextMap[inputType];
}

// Updates interaction widget display
void UInteractableController::UpdateWidgetDisplay()
{
	if (!InteractableWidget) return;

	bool wasInViewport = InteractableWidget->IsInViewport();

	// Remove from viewport if no interactable is present
	if (!BestInteractable)
	{
		if (wasInViewport)
			InteractableWidget->RemoveFromParent();
		return;
	}

	// Update display text
	IInteractableInterface::Execute_UpdatePrimaryPrompt(InteractableWidget, BestInteractable->PrimaryActionEnabled, GetPrimaryInputDisplayText(ActiveInputType), BestInteractable->PrimaryActionText);
	IInteractableInterface::Execute_UpdateSecondaryPrompt(InteractableWidget, BestInteractable->SecondaryActionEnabled, GetSecondaryInputDisplayText(ActiveInputType), BestInteractable->SecondaryActionText);

	// Add to viewport
	if (!wasInViewport)
		InteractableWidget->AddToViewport();
}

// Detect input changed and update any visible interactables
void UInteractableController::OnInputChanged(ECommonInputType newInputType)
{
	ActiveInputType = newInputType;
	UpdateWidgetDisplay();
}

// Handler function for triggering interactable
void UInteractableController::OnPrimaryInteractPressed()
{
	if (!BestInteractable)
		return;

	BestInteractable->TriggerPrimary(this);
	UpdateWidgetDisplay();
}

// Handler function for triggering interactable
void UInteractableController::OnSecondaryInteractPressed()
{
	if (!BestInteractable)
		return;

	BestInteractable->TriggerSecondary(this);
	UpdateWidgetDisplay();
}

// Function to register an interactable as active
void UInteractableController::RegisterInteractable(UInteractable* interactable)
{
	ActiveInteractables.Add(interactable);
	UpdateBestInteractable();
}

// Function to unregister an interactable
void UInteractableController::UnregisterInteractable(UInteractable* interactable)
{
	int32 count = ActiveInteractables.Remove(interactable);
	if (count == 0) return;

	if (!interactable || interactable == BestInteractable)
		return;

	UpdateBestInteractable();
}

