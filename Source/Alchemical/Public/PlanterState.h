#pragma once

#include "CoreMinimal.h"
#include "PlanterState.generated.h"

/**
 * Enum representing the planter state.
 */
UENUM(BlueprintType)
enum class EPlanterState : uint8
{
    Empty UMETA(DisplayName = "Empty"),
    Growing UMETA(DisplayName = "Growing"),
    Harvestable UMETA(DisplayName = "Harvestable")
};