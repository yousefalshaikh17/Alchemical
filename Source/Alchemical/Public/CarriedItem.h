#pragma once

#include "CoreMinimal.h"
#include "CarriedItem.generated.h"

/**
 * Enum representing the player carried item.
 */
UENUM(BlueprintType)
enum class ECarriedItem : uint8
{
    None UMETA(DisplayName = "None"),
    Seed UMETA(DisplayName = "Seed"),
    Plant UMETA(DisplayName = "Plant")
};