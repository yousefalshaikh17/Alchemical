#pragma once

#include "CoreMinimal.h"
#include "GamePhase.generated.h"

/**
 * Enum representing the game phase.
 */
UENUM(BlueprintType)
enum class EGamePhase : uint8
{
    MainMenu UMETA(DisplayName = "Main Menu"),
    Playing UMETA(DisplayName = "Playing"),
    Paused UMETA(DisplayName = "Paused"),
    LevelEnd UMETA(DisplayName = "Level End"),
    GameOver UMETA(DisplayName = "Game Over")
};