// Fill out your copyright notice in the Description page of Project Settings.


#include "SeedBox.h"

#include "PlayerCharacter.h"

// Sets default values
ASeedBox::ASeedBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASeedBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractableComponent = GetComponentByClass<UInteractable>();
	if (InteractableComponent)
	{
		InteractableComponent->OnPrimaryInteract.AddDynamic(this, &ASeedBox::OnPrimaryInteract);
		USceneComponent* AttachParent = InteractableComponent->GetAttachParent();
		if (UShapeComponent* Collider = Cast<UShapeComponent>(AttachParent))
			Collider->OnComponentBeginOverlap.AddDynamic(this, &ASeedBox::OnColliderBeginOverlap);
	}
}

// Called when the game starts or when spawned
void ASeedBox::BeginPlay()
{
	Super::BeginPlay();
}

void ASeedBox::OnPrimaryInteract_Implementation(UInteractable* Interactable, AActor* ActorWhoTriggered,
UInteractableController* ControllerThatTriggered)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ActorWhoTriggered);
	if (!PlayerCharacter) return;

	FItemInstance Item;
	PlayerCharacter->GetHeldItem(Item);
	
	switch (Item.Type)
	{
	case EItemType::None:
		GiveSeed(PlayerCharacter);
		break;
	case EItemType::Seed:
		TakeSeed(PlayerCharacter);
		break;
	default:
		break;
	}
}

int ASeedBox::GetPlantIndex() const
{
	switch (SeedType)
	{
	case EElement::Death:
		return 1;
	case EElement::Soul:
		return 2;
	case EElement::Magic:
		return 3;
	}
	return -1;
}

void ASeedBox::UpdateInteractableState(const EItemType& CarriedItemType) const
{
	switch (CarriedItemType)
	{
	case EItemType::None:
		InteractableComponent->PrimaryActionText = FText::FromString("Take Seed");
		InteractableComponent->PrimaryActionEnabled = true;
		break;
	case EItemType::Seed:
		InteractableComponent->PrimaryActionText = FText::FromString("Return Seed");
		InteractableComponent->PrimaryActionEnabled = true;
		break;
	default:
		InteractableComponent->PrimaryActionEnabled = false;
		break;
	}
}

void ASeedBox::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;

	FItemInstance Item;
	PlayerCharacter->GetHeldItem(Item);
	UpdateInteractableState(Item.Type);
}

void ASeedBox::TakeSeed_Implementation(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UpdateHeldItem(FItemInstance{});

	FItemInstance Item;
	PlayerCharacter->GetHeldItem(Item);
	UpdateInteractableState(Item.Type);
}

void ASeedBox::GiveSeed_Implementation(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UpdateHeldItem(FItemInstance{
		EItemType::Seed,
		GetPlantIndex()
	});

	FItemInstance Item;
	PlayerCharacter->GetHeldItem(Item);
	UpdateInteractableState(Item.Type);
}
