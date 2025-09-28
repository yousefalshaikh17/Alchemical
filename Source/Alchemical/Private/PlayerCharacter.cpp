// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameDataSubsystem.h"
#include "ItemContainerComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	const APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (!Controller) return;

	const APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager) return;

	CameraTransform = CameraManager->GetTransformComponent();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::GetHeldPlantData(bool& bFoundPlant, FPlantData& PlantData) const
{
	bFoundPlant = false;
	
	if (!ItemContainerComponent) return;
	
	ItemContainerComponent->GetItemPlantData(bFoundPlant, PlantData);
}

void APlayerCharacter::GetHeldItem(FItemInstance& Item) const
{
	if (!ItemContainerComponent) return;
	
	ItemContainerComponent->GetItem(Item);
}


void APlayerCharacter::Move_Implementation(const float RightScale, const float ForwardScale)
{
	if (!bMovementEnabled) return;
	if (!CameraTransform) return;

	AddMovementInput(CameraTransform->GetRightVector(), RightScale, false);
	AddMovementInput(CameraTransform->GetForwardVector(), ForwardScale, false);
}

void APlayerCharacter::UpdateHeldItem_Implementation(const FItemInstance NewItem)
{
	if (!ItemContainerComponent) return;
	
	if (!ItemContainerComponent->SetItem(NewItem)) return;
	
	// Broadcast
	OnHeldItemChanged.Broadcast(this, NewItem);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveInputAction)
			EnhancedInput->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleMoveInput);
	}
}

void APlayerCharacter::HandleMoveInput_Implementation(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	Move(MoveValue.X, MoveValue.Y);
}
