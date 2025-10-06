// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemicalPlayerController.h"
#include "AlchemicalGameState.h"
#include "AlchemicalGameMode.h"
#include "EnhancedInputComponent.h"

AAlchemicalPlayerController::AAlchemicalPlayerController()
{
	GameMusic = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("GameMusic"));
	GameMusic->SetAutoActivate(false);
	
	MenuMusic = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("MenuMusic"));
	MenuMusic->SetAutoActivate(false);
}

void AAlchemicalPlayerController::OnHealthChanged_Implementation(const int NewHealth)
{
	if (!HudWidget) return;

	HudWidget->UpdateHealth(NewHealth);
	UpdateGameMusicPower();
}

void AAlchemicalPlayerController::OnScoreChanged_Implementation(const int NewScore)
{
	if (!HudWidget) return;

	HudWidget->UpdateScore(NewScore);
}

void AAlchemicalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind pause keybind
	InitializePlayerInput();
	
	// Initialize Widgets
	InitializeWidgets();
	
	// Bind Level Change
	GameState->OnLevelChanged.AddDynamic(this, &AAlchemicalPlayerController::LevelChanged);

	// Bind player stats
	GameState->OnHealthLost.AddDynamic(this, &AAlchemicalPlayerController::OnHealthLost);
	GameState->OnScoreGained.AddDynamic(this, &AAlchemicalPlayerController::OnScoreGained);
	
	// Bind Phase Change
	GameState->OnPhaseChanged.AddDynamic(this, &AAlchemicalPlayerController::GamePhaseChanged);
	GamePhaseBegin(GameState->CurrentPhase);

	// If starting with main menu, pause after a frame to allow the world to tick once.
	if (GameState->CurrentPhase == EGamePhase::MainMenu)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			PauseGame();
		});
	}
}

template <typename T>
T* AAlchemicalPlayerController::CreateAndAddWidget(TSubclassOf<T> WidgetClass)
{
	if (!WidgetClass) return nullptr;

	T* Widget = CreateWidget<T>(this, WidgetClass);
	if (Widget)
		Widget->AddToViewport();

	return Widget;
}

void AAlchemicalPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const UWorld* World = GetWorld();
	GameMode = World->GetAuthGameMode<AAlchemicalGameMode>();
	GameState = World->GetGameState<AAlchemicalGameState>();
}

void AAlchemicalPlayerController::RequestLevelPause(const bool bShouldPause) const
{
	if (!GameState || !GameMode) return;

	const EGamePhase ExpectedCurrentPhase = (bShouldPause) ? EGamePhase::Playing : EGamePhase::Paused;
	
	if (GameState->CurrentPhase == ExpectedCurrentPhase)
		GameMode->SetPaused(bShouldPause);
}

void AAlchemicalPlayerController::RequestMainMenu() const
{
	if (!GameMode) return;

	GameMode->ReturnToMainMenu();
}

void AAlchemicalPlayerController::RequestStartLevel(const int LevelIndexOverride) const
{
	if (!GameMode) return;

	GameMode->StartLevel(LevelIndexOverride);
}

void AAlchemicalPlayerController::RequestNextLevel() const
{
	if (!GameMode) return;

	GameMode->NextLevel();
}

void AAlchemicalPlayerController::InitializeWidgets_Implementation()
{
	MainMenuWidget = CreateAndAddWidget(MainMenuWidgetClass);
	PauseMenuWidget = CreateAndAddWidget(PauseMenuWidgetClass);
	GameOverWidget = CreateAndAddWidget(GameOverWidgetClass);
	LevelCompleteWidget = CreateAndAddWidget(LevelCompleteWidgetClass);
	HudWidget = CreateAndAddWidget(HudWidgetClass);
}

void AAlchemicalPlayerController::SetWidgetVisibility(UUserWidget* Widget, const bool bVisible)
{
	if (!IsValid(Widget)) return;
	
	const ESlateVisibility Visibility = (bVisible) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	Widget->SetVisibility(Visibility);
}

void AAlchemicalPlayerController::SetMenuMusicVolume(const float NewVolume)
{
	MenuMusicVolume = NewVolume;
	MenuMusic->SetVolume(NewVolume);
}

void AAlchemicalPlayerController::SetGameMusicVolume(const float NewVolume)
{
	GameMusicVolume = NewVolume;
	GameMusic->SetVolume(NewVolume);
}

void AAlchemicalPlayerController::UpdateGameMusicPower() const
{
	if (!GameState) return;
	
	GameMusic->SetParameter("Power", 3 - GameState->HealthPoints);
}

bool AAlchemicalPlayerController::IsLevelEnded() const
{
	if (!GameState) return false;
	
	return GameState->CurrentPhase == EGamePhase::GameOver || GameState->CurrentPhase == EGamePhase::LevelEnd;
}

void AAlchemicalPlayerController::GamePhaseBegin_Implementation(const EGamePhase NewPhase)
{
	switch (NewPhase)
	{
	case EGamePhase::MainMenu:
		SetWidgetVisibility(MainMenuWidget, true);
		break;
	case EGamePhase::Playing:
		UnpauseGame();
		SetWidgetVisibility(HudWidget, true);
		break;
	case EGamePhase::Paused:
		SetWidgetVisibility(PauseMenuWidget, true);
		break;
	case EGamePhase::LevelEnd:
		HandleLevelEndPhaseBegin();
		break;
	case EGamePhase::GameOver:
		SetWidgetVisibility(GameOverWidget, true);
		GameMusic->Stop();
		break;
	}
}

void AAlchemicalPlayerController::GamePhaseEnd_Implementation(const EGamePhase OldPhase)
{
	switch (OldPhase)
	{
	case EGamePhase::MainMenu:
		SetWidgetVisibility(MainMenuWidget, false);
		break;
	case EGamePhase::Playing:
		HandlePlayingPhaseEnd();
		break;
	case EGamePhase::Paused:
		SetWidgetVisibility(PauseMenuWidget, false);
		break;
	case EGamePhase::LevelEnd:
		SetWidgetVisibility(LevelCompleteWidget, false);
		break;
	case EGamePhase::GameOver:
		SetWidgetVisibility(GameOverWidget, false);
		break;
	}
}

void AAlchemicalPlayerController::GamePhaseChanged(const EGamePhase OldPhase, const EGamePhase NewPhase)
{
	GamePhaseEnd(OldPhase);

	// If Playing without being in Pause Menu prior, reset music.
	if (NewPhase == EGamePhase::Playing && OldPhase != EGamePhase::Paused)
	{
		GameMusic->SetEvent(GameMusic->Event);
		GameMusic->SetParameter("Power", 0);
	}
	
	GamePhaseBegin(NewPhase);
}

void AAlchemicalPlayerController::LevelChanged_Implementation(const int NewLevelIndex, const FLevelData LevelData)
{
	HudWidget->UpdateLevelData(NewLevelIndex + 1, LevelData.RequiredNPCsToComplete);
}

void AAlchemicalPlayerController::OnHealthLost_Implementation()
{
}

void AAlchemicalPlayerController::OnScoreGained_Implementation()
{
}

void AAlchemicalPlayerController::PlayerTogglePause_Implementation()
{
	if (!GameState) return;
	
	RequestLevelPause(GameState->CurrentPhase == EGamePhase::Playing);
}

void AAlchemicalPlayerController::InitializePlayerInput_Implementation()
{
	// Add Mapping Context
	if (InputMappingContext)
	{
		const ULocalPlayer* LocalPlayer = GetLocalPlayer();
		if (!LocalPlayer) return;

		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (!Subsystem) return;

		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInput) return;
	
	// Bind Pause
	if (PauseInputAction)
	{
		EnhancedInput->BindAction(
			PauseInputAction,
			ETriggerEvent::Started,
			this,
			&AAlchemicalPlayerController::PlayerTogglePause
		);
	}
}

void AAlchemicalPlayerController::HandlePlayingPhaseEnd()
{
	const bool bGameEnded = IsLevelEnded();

	if (bGameEnded)
		GameMusic->Stop(); // Play outro
	
	PauseGame(!bGameEnded);
	SetWidgetVisibility(HudWidget, false);
}

void AAlchemicalPlayerController::HandleLevelEndPhaseBegin() const
{
	const UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!GameDataSubsystem) return;

	// TODO: Update subsystem to have a way to retrieve number of levels without allocating a whole array.
	TArray<FLevelData> LevelDataArray;
	GameDataSubsystem->GetAllLevelData(LevelDataArray);
	const int MaxLevelIndex = LevelDataArray.Num() - 1;
	
	LevelCompleteWidget->FinalLevelCompleted = MaxLevelIndex == GameState->LevelIndex;
	SetWidgetVisibility(LevelCompleteWidget, true);
	GameMusic->Stop();
}

void AAlchemicalPlayerController::PauseGame_Implementation(const bool bPauseGameMusic)
{
	if (!IsPaused() && SetPause(true))
		PlayMenuMusic(bPauseGameMusic);
}

void AAlchemicalPlayerController::UnpauseGame_Implementation()
{
	if (IsPaused() && SetPause(false))
		PlayGameMusic();
}

void AAlchemicalPlayerController::PlayMenuMusic_Implementation(const bool bPauseGameMusic)
{
	GameMusic->SetPaused(bPauseGameMusic);
	MenuMusic->SetVolume(MenuMusicVolume);
	MenuMusic->Play();
}

void AAlchemicalPlayerController::PlayGameMusic_Implementation()
{
	GameMusic->SetPaused(false);
	MenuMusic->Stop();

	if (GameMusic->IsPlaying()) return;

	GameMusic->SetTimelinePosition(0);
	GameMusic->SetVolume(GameMusicVolume);
	GameMusic->Play();
}
