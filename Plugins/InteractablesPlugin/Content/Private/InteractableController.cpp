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
		enhancedInput->BindAction(InteractInputAction, ETriggerEvent::Started, this, &UInteractableController::OnInteractPressed);
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
		// If there is no candidate, initialize with the current candidate.
		if (!bestCandidate)
		{
			bestCandidate = interactable;
			continue;
		}

		// Get distance squared. Distance squared saves computation time as it avoids the sqrt() operation.
		FVector location = interactable->GetComponentLocation();
		float distanceSqaured = FVector::DistSquared(actorLocation, location);

		// If distance is less than the current minimum, update it.
		if (distanceSqaured < minDistanceSquared)
		{
			minDistanceSquared = distanceSqaured;
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

	BestInteractable = newBest;
	UpdateWidgetDisplay();
} 

// Given an input type, returns the user-facing display text
FText UInteractableController::GetInputDisplayText(ECommonInputType inputType)
{
	bool hasDisplayText = InputDisplayTextMap.Contains(inputType);
	if (!hasDisplayText) return UnknownInputDisplayText;

	return InputDisplayTextMap[inputType];
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
	IInteractableInterface::Execute_UpdateText(InteractableWidget, GetInputDisplayText(ActiveInputType), BestInteractable->ActionText);

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
void UInteractableController::OnInteractPressed()
{
	if (!BestInteractable)
		return;

	BestInteractable->Trigger(this);
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

