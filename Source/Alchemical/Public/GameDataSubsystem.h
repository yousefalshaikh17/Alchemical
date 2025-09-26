// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PlantData.h"
#include "LevelData.h"

#include "GameDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMICAL_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UGameDataSubsystem();

	UFUNCTION(BlueprintPure, Category = "PlantData")
	void GetPlantData(int PlantIndex, FPlantData& PlantData, bool& bFoundPlant) const;

	UFUNCTION(BlueprintPure, Category = "PlantData")
	void GetAllPlantData(TArray<FPlantData>& PlantDataArray) const;

	UFUNCTION(BlueprintPure, Category = "LevelData")
	void GetLevelData(int LevelIndex, FLevelData& LevelData, bool& bFoundLevel) const;
	
	UFUNCTION(BlueprintPure, Category = "LevelData")
	void GetAllLevelData(TArray<FLevelData>& LevelDataArray) const;

private:
    UPROPERTY()
    UDataTable* LevelDataTable;

    UPROPERTY()
    UDataTable* PlantDataTable;
};
