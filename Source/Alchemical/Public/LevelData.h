#pragma once

#include "CoreMinimal.h"

#include "LevelData.generated.h"

/**
 * Plant Type Row which provides data on a plant.
 */
USTRUCT(BlueprintType)
struct FLevelData: public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="PlantIndexSelection"))
	TArray<int32> PlantIndexSelection;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="NPCsToComplete", ClampMin="1", MakeStructureDefaultValue="0"))
	int32 RequiredNPCsToComplete;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="NPCSpeed", ClampMin="0", MakeStructureDefaultValue="0.000000"))
	double NPCSpeed;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SpawningIntervalSeconds", ClampMin="0", MakeStructureDefaultValue="0.000000"))
	double SpawningIntervalSeconds;
};
