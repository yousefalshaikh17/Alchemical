// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameDataSubsystem.h"

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

bool APlayerCharacter::GetHeldPlantData(FPlantData& PlantData) const
{
	if (PlantIndex < 0) return false;
	
	const UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!GameDataSubsystem) return false;

	bool bFoundPlant = false;
	GameDataSubsystem->GetPlantData(PlantIndex, PlantData, bFoundPlant);
	
	return bFoundPlant;
}

void APlayerCharacter::UpdateHeldItemDisplay() const
{
	if (CarriedItemType == EItemType::None)
	{
		if (ElementsDisplayWidget)
			ElementsDisplayWidget->ClearElementDisplay();

		if (PlantSpriteComponent)
			PlantSpriteComponent->SetVisibility(false);
		return;
	}

	if (FPlantData PlantData; GetHeldPlantData(PlantData))
	{
		UPaperSprite* NewSprite;
		switch (CarriedItemType)
		{
		case EItemType::Seed:
			NewSprite = SeedSprite;
			break;
		case EItemType::Plant:
			NewSprite = PlantData.Sprite;
			break;
		default:
			NewSprite = nullptr;
		}

		if (ElementsDisplayWidget)
			ElementsDisplayWidget->UpdateElementDisplay(PlantData.Elements);

		if (PlantSpriteComponent)
		{
			PlantSpriteComponent->SetSprite(NewSprite);
			PlantSpriteComponent->SetVisibility(true);	
		}
	}
}

bool APlayerCharacter::IsSameCarriedItem(const EItemType NewCarriedItemType, const int NewPlantIndex) const
{
	return CarriedItemType == NewCarriedItemType && (CarriedItemType == EItemType::None || PlantIndex == NewPlantIndex);
}

void APlayerCharacter::Move_Implementation(const float RightScale, const float ForwardScale)
{
	if (!bMovementEnabled) return;
	if (!CameraTransform) return;

	AddMovementInput(CameraTransform->GetRightVector(), RightScale, false);
	AddMovementInput(CameraTransform->GetForwardVector(), ForwardScale, false);
}

void APlayerCharacter::UpdateHeldItem_Implementation(const EItemType NewCarriedItemType, const int NewPlantIndex)
{
	if (IsSameCarriedItem(NewCarriedItemType, NewPlantIndex))
		return;
	
	CarriedItemType = NewCarriedItemType;
	PlantIndex = NewPlantIndex;
	
	// Handle Sprite
	UpdateHeldItemDisplay();

	// Broadcast
	OnHeldItemChanged.Broadcast(this, CarriedItemType, PlantIndex);
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
