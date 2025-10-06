// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemicalGameState.h"
#include "AlchemicalPlayerController.h"

void AAlchemicalGameState::OnRep_Score()
{
	AAlchemicalPlayerController* Controller = GetWorld()->GetFirstPlayerController<AAlchemicalPlayerController>();
	if (!Controller) return;

	Controller->OnScoreChanged(Score);
}

void AAlchemicalGameState::OnRep_HealthPoints()
{
	AAlchemicalPlayerController* Controller = GetWorld()->GetFirstPlayerController<AAlchemicalPlayerController>();
	if (!Controller) return;

	Controller->OnHealthChanged(HealthPoints);
}

void AAlchemicalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAlchemicalGameState, HealthPoints)
	DOREPLIFETIME(AAlchemicalGameState, Score)
}

void AAlchemicalGameState::SetHealth_Implementation(const int NewHealth)
{
	if (NewHealth == HealthPoints) return;

	HealthPoints = NewHealth;
	
	if (GetNetMode() == NM_Standalone) // For standalone testing
		OnRep_HealthPoints();
		
}

void AAlchemicalGameState::SetScore_Implementation(const int NewScore)
{
	if (NewScore == Score) return;

	Score = NewScore;

	if (GetNetMode() == NM_Standalone) // For standalone testing
		OnRep_Score();
}

void AAlchemicalGameState::RemoveLife_Implementation()
{
	SetHealth(HealthPoints - 1);
	OnHealthLost.Broadcast();
}

void AAlchemicalGameState::AddScore_Implementation(const int ScoreToAdd)
{
	SetScore(Score + ScoreToAdd);
	OnScoreGained.Broadcast();
}

void AAlchemicalGameState::ChangeLevel_Implementation(const int NewLevelIndex)
{
	const UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!GameDataSubsystem) return;

	FLevelData LevelData;
	bool bFoundLevel = false;
	GameDataSubsystem->GetLevelData(NewLevelIndex, LevelData, bFoundLevel);

	if (!bFoundLevel) return;

	LevelIndex = NewLevelIndex;
	RequiredScore = LevelData.RequiredNPCsToComplete;

	OnLevelChanged.Broadcast(LevelIndex, LevelData);
}

void AAlchemicalGameState::SetGamePhase_Implementation(const EGamePhase NewPhase)
{
	if (NewPhase == CurrentPhase) return;

	const EGamePhase OldPhase = CurrentPhase;
	CurrentPhase = NewPhase;

	OnPhaseChanged.Broadcast(OldPhase, NewPhase);
}
