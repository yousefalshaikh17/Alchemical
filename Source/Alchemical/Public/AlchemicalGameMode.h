// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemicalGameState.h"
#include "GameFramework/GameModeBase.h"
#include "AlchemicalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMICAL_API AAlchemicalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void ResetLevel() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RegisterResettableActor(AActor* Actor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnregisterResettableActor(const AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnToMainMenu() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartLevel(int LevelIndexOverride) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NextLevel() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPaused(bool bPaused) const;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void AddScore(const int ScoreToAdd) const;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void RemoveLife() const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AAlchemicalGameState* GameStateInstance;
	
	TSet<TWeakObjectPtr<AActor>> ResettableActors;

	int MaxLevelIndex;

	UFUNCTION(BlueprintNativeEvent)
	void GamePhaseChanged(EGamePhase OldPhase, EGamePhase NewPhase);
};
