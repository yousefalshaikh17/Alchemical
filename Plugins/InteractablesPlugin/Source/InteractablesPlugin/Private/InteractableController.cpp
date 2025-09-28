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

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(PlayerController->GetLocalPlayer());
	
	// Bind input changed
	CommonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &UInteractableController::OnInputChanged);
	// Initialize input type
	ActiveInputType = CommonInputSubsystem->GetCurrentInputType();

	// Set up player input
	const AActor* Owner = GetOwner();
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(Owner->InputComponent))
	{
		EnhancedInput->BindAction(PrimaryInteractInputAction, ETriggerEvent::Started, this, &UInteractableController::OnPrimaryInteractPressed);
		EnhancedInput->BindAction(SecondaryInteractInputAction, ETriggerEvent::Started, this, &UInteractableController::OnSecondaryInteractPressed);
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

	float MinDistanceSquared = INFINITY;
	UInteractable* BestCandidate = nullptr;

	const FVector ActorLocation = GetOwner()->GetActorLocation();

	for (const auto& Interactable : ActiveInteractables)
	{
		if (!Interactable->PrimaryActionEnabled && !Interactable->SecondaryActionEnabled) continue;

		// If there is no candidate, initialize with the current candidate.
		if (!BestCandidate)
		{
			BestCandidate = Interactable;
			continue;
		}

		// Get distance squared. Distance squared saves computation time as it avoids the sqrt() operation.
		FVector Location = Interactable->GetComponentLocation();
		const float DistanceSquared = FVector::DistSquared(ActorLocation, Location);

		// If distance is less than the current minimum, update it.
		if (DistanceSquared < MinDistanceSquared)
		{
			MinDistanceSquared = DistanceSquared;
			BestCandidate = Interactable;
		}
	}

	return BestCandidate;
}

// Updates best interactable variable and display
void UInteractableController::UpdateBestInteractable()
{
	UInteractable* NewBest = GetClosestInteractable();
	if (NewBest == BestInteractable)
		return;

	if (BestInteractable)
	{
		BestInteractable->DisplayingController = nullptr;
		BestInteractable->OnPromptHidden.Broadcast(NewBest, GetOwner(), this);
	}

	if (NewBest)
	{
		NewBest->DisplayingController = this;
		NewBest->OnPromptShown.Broadcast(NewBest, GetOwner(), this);
	}

	BestInteractable = NewBest;
	UpdateWidgetDisplay();
} 

// Given an input type, returns the user-facing display text
FText UInteractableController::GetPrimaryInputDisplayText(const ECommonInputType InputType)
{
	const bool bHasDisplayText = PrimaryInputDisplayTextMap.Contains(InputType);
	if (!bHasDisplayText) return UnknownInputDisplayText;

	return PrimaryInputDisplayTextMap[InputType];
}

FText UInteractableController::GetSecondaryInputDisplayText(const ECommonInputType InputType)
{
	const bool bHasDisplayText = SecondaryInputDisplayTextMap.Contains(InputType);
	if (!bHasDisplayText) return UnknownInputDisplayText;

	return SecondaryInputDisplayTextMap[InputType];
}

// Updates interaction widget display
void UInteractableController::UpdateWidgetDisplay()
{
	if (!InteractableWidget) return;

	const bool bWasInViewport = InteractableWidget->IsInViewport();

	// Remove from viewport if no interactable is present
	if (!BestInteractable)
	{
		if (bWasInViewport)
			InteractableWidget->RemoveFromParent();
		return;
	}

	// Update display text
	IInteractableInterface::Execute_UpdatePrimaryPrompt(InteractableWidget, BestInteractable->PrimaryActionEnabled, GetPrimaryInputDisplayText(ActiveInputType), BestInteractable->PrimaryActionText);
	IInteractableInterface::Execute_UpdateSecondaryPrompt(InteractableWidget, BestInteractable->SecondaryActionEnabled, GetSecondaryInputDisplayText(ActiveInputType), BestInteractable->SecondaryActionText);

	// Add to viewport
	if (!bWasInViewport)
		InteractableWidget->AddToViewport();
}

// Detect input changed and update any visible interactables
void UInteractableController::OnInputChanged(const ECommonInputType NewInputType)
{
	ActiveInputType = NewInputType;
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
void UInteractableController::RegisterInteractable(UInteractable* Interactable)
{
	ActiveInteractables.Add(Interactable);
	UpdateBestInteractable();
}

// Function to unregister an interactable
void UInteractableController::UnregisterInteractable(UInteractable* Interactable)
{
	const int32 Count = ActiveInteractables.Remove(Interactable);
	if (Count == 0) return;

	if (!Interactable || Interactable == BestInteractable)
		return;

	UpdateBestInteractable();
}

