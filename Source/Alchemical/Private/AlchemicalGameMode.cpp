// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemicalGameMode.h"

void AAlchemicalGameMode::RegisterResettableActor_Implementation(AActor* Actor)
{
	ResettableActors.Add(Actor);
}

void AAlchemicalGameMode::UnregisterResettableActor_Implementation(const AActor* Actor)
{
	ResettableActors.Remove(Actor);
}

void AAlchemicalGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameStateInstance = GetWorld()->GetGameState<AAlchemicalGameState>();
	if (GameStateInstance)
	{
		GameStateInstance->OnPhaseChanged.AddDynamic(this, &AAlchemicalGameMode::GamePhaseChanged);
	}
	
	const UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!GameDataSubsystem) return;

	// TODO: Add Count function to subsystem.
	// This isn't a big concern since its a small array and only on BeginPlay, but could still be accounted for.
	TArray<FLevelData> LevelDataArray;
	GameDataSubsystem->GetAllLevelData(LevelDataArray);
	MaxLevelIndex = LevelDataArray.Num() - 1;
}

void AAlchemicalGameMode::ResetLevel()
{
	for (const auto WeakActor : ResettableActors)
	{
		if (AActor* Actor = WeakActor.Get())
			Actor->Reset();
	}
}

void AAlchemicalGameMode::ReturnToMainMenu_Implementation() const
{
	if (!GameStateInstance) return;
	GameStateInstance->SetGamePhase(EGamePhase::MainMenu);
}

void AAlchemicalGameMode::RemoveLife_Implementation() const
{
	if (!GameStateInstance) return;

	GameStateInstance->RemoveLife();

	// Check if player should still be alive
	if (GameStateInstance->HealthPoints > 0) return;

	GameStateInstance->SetGamePhase(EGamePhase::GameOver);
}

void AAlchemicalGameMode::AddScore_Implementation(const int ScoreToAdd) const
{
	if (!GameStateInstance) return;

	GameStateInstance->AddScore(ScoreToAdd);

	// Check if player has met required score
	if (GameStateInstance->Score < GameStateInstance->RequiredScore) return;

	GameStateInstance->SetGamePhase(EGamePhase::LevelEnd);
}

void AAlchemicalGameMode::StartLevel_Implementation(const int LevelIndexOverride) const
{
	if (!GameStateInstance) return;

	const int NewLevel = (LevelIndexOverride > -1) ? LevelIndexOverride : GameStateInstance->LevelIndex;

	GameStateInstance->ChangeLevel(NewLevel);

	GameStateInstance->SetHealth(3);
	GameStateInstance->SetScore(0);

	GameStateInstance->SetGamePhase(EGamePhase::Playing);
}

void AAlchemicalGameMode::NextLevel_Implementation() const
{
	if (!GameStateInstance) return;

	// Validate that the player is done with the level
	if (GameStateInstance->Score < GameStateInstance->RequiredScore)
		return;

	// Progress to level if not hit the final level
	const int NewLevelIndex = GameStateInstance->LevelIndex + 1;
	if (NewLevelIndex > MaxLevelIndex)
		return;

	StartLevel(NewLevelIndex);
}

void AAlchemicalGameMode::SetPaused_Implementation(const bool bPaused) const
{
	if (!GameStateInstance) return;
	
	const EGamePhase NewPhase = (bPaused) ? EGamePhase::Paused : EGamePhase::Playing;
	GameStateInstance->SetGamePhase(NewPhase);
}

void AAlchemicalGameMode::GamePhaseChanged_Implementation(EGamePhase OldPhase, EGamePhase NewPhase)
{
	switch (NewPhase)
	{
		case EGamePhase::MainMenu:
		case EGamePhase::LevelEnd:
		case EGamePhase::GameOver:
			ResetLevel();
			break;
		default:
			break;
	}
}
