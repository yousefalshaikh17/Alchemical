// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GamePhase.h"
#include "LevelData.h"
#include "GameDataSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "AlchemicalGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPhaseChanged, EGamePhase, OldPhase, EGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32, LevelIndex, FLevelData, LevelData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStatChanged);

/**
 * Class to handle the game state for the level.
 */
UCLASS()
class ALCHEMICAL_API AAlchemicalGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnRep_Score();

	UFUNCTION()
	void OnRep_HealthPoints();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	
	// Player Data
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data", ReplicatedUsing="OnRep_HealthPoints", meta=(ClampMin="0", UIMin="0"))
	int HealthPoints;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data", ReplicatedUsing="OnRep_Score", meta=(ClampMin="0", UIMin="0"))
	int Score;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category="Data")
	FOnPlayerStatChanged OnHealthLost;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category="Data")
	FOnPlayerStatChanged OnScoreGained;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void AddScore(int ScoreToAdd);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void RemoveLife();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void SetScore(const int NewScore);
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void SetHealth(const int NewHealth);

	
	// Level management
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Level")
	EGamePhase CurrentPhase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level")
	int32 LevelIndex;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Level")
	int32 RequiredScore;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Level")
	FOnLevelChanged OnLevelChanged;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Level")
	FOnPhaseChanged OnPhaseChanged;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable, Category="Level")
	void SetGamePhase(const EGamePhase NewPhase);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable, Category="Level")
	void ChangeLevel(const int NewLevelIndex);
};