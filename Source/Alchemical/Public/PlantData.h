#pragma once

#include "CoreMinimal.h"
#include "Element.h"
#include "PaperSprite.h"

#include "PlantData.generated.h"

/**
 * Plant Type Row which provides data on a plant.
 */
USTRUCT(BlueprintType)
struct FPlantData: public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "PlantName"))
	FString PlantName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Elements"))
	TSet<EElement> Elements;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "PointsValue"))
	int32 PointsValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GrowthTime"))
	double GrowthTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Sprite"))
	TSoftObjectPtr<UPaperSprite> Sprite;
};
