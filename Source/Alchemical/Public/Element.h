#pragma once

#include "CoreMinimal.h"
#include "Element.generated.h"

/**
 * Enum representing the element types.
 */
UENUM(BlueprintType)
enum class EElement : uint8
{
    Death UMETA(DisplayName = "Death"),
    Soul UMETA(DisplayName = "Soul"),
    Magic UMETA(DisplayName = "Magic")
};