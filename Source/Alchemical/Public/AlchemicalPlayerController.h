// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FMODAudioComponent.h"
#include "GamePhase.h"
#include "HUDWidget.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "LevelCompleteWidget.h"
#include "LevelData.h"
#include "PlayerCharacter.h"
#include "AlchemicalPlayerController.generated.h"

class AAlchemicalGameMode;
class AAlchemicalGameState;

/**
 * Custom player controller
 */
UCLASS()
class ALCHEMICAL_API AAlchemicalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAlchemicalPlayerController();
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnScoreChanged(int NewScore);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHealthChanged(int NewHealth);


	// Requests (For interfacing with GameMode)
	
	UFUNCTION(BlueprintCallable)
	void RequestLevelPause(const bool bShouldPause) const;

	UFUNCTION(BlueprintCallable)
	void RequestMainMenu() const;

	UFUNCTION(BlueprintCallable)
	void RequestStartLevel(const int LevelIndexOverride = -1) const;

	UFUNCTION(BlueprintCallable)
	void RequestNextLevel() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AAlchemicalGameState* GameState;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AAlchemicalGameMode* GameMode;

	// Input

	UPROPERTY(EditDefaultsOnly)
	UInputAction* PauseInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext;

	UFUNCTION(BlueprintNativeEvent)
	void InitializePlayerInput();
	
	UFUNCTION(BlueprintNativeEvent)
	void PlayerTogglePause();
	
	// Widget setup
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UUserWidget* MainMenuWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UUserWidget* PauseMenuWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UUserWidget* GameOverWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ULevelCompleteWidget* LevelCompleteWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UHUDWidget* HudWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ULevelCompleteWidget> LevelCompleteWidgetClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UHUDWidget> HudWidgetClass;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeWidgets();
	
	UFUNCTION(BlueprintCallable)
	static void SetWidgetVisibility(UUserWidget* Widget, const bool bVisible);

	// Game Music
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Audio")
	int GameMusicVolume = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Audio")
	int MenuMusicVolume = 1;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Audio")
	UFMODAudioComponent* GameMusic;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Audio")
	UFMODAudioComponent* MenuMusic;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Audio")
	void PlayGameMusic();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Audio")
	void PlayMenuMusic(bool bPauseGameMusic = true);

	UFUNCTION(BlueprintCallable, Category="Audio")
	void SetMenuMusicVolume(float NewVolume = 1);

	UFUNCTION(BlueprintCallable, Category="Audio")
	void SetGameMusicVolume(float NewVolume = 1);

	UFUNCTION(BlueprintCallable)
	void UpdateGameMusicPower() const;
	
	// Pause State
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PauseGame(const bool bPauseGameMusic = true);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnpauseGame();

	// Game Phase Management
	
	UFUNCTION()
	void GamePhaseChanged(const EGamePhase OldPhase, const EGamePhase NewPhase);

	UFUNCTION(BlueprintNativeEvent)
	void GamePhaseBegin(const EGamePhase NewPhase);

	UFUNCTION(BlueprintNativeEvent)
	void GamePhaseEnd(const EGamePhase OldPhase);

	// Level management
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelChanged(const int NewLevelIndex, const FLevelData LevelData);

	UFUNCTION(BlueprintPure)
	bool IsLevelEnded() const;
	
	// Player Stat Changes
	
	UFUNCTION(BlueprintNativeEvent)
	void OnHealthLost();

	UFUNCTION(BlueprintNativeEvent)
	void OnScoreGained();
private:
	template<typename T>
	T* CreateAndAddWidget(TSubclassOf<T> WidgetClass);

	void HandlePlayingPhaseEnd();
	void HandleLevelEndPhaseBegin() const;
};
