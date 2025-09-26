// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataSubsystem.h"

UGameDataSubsystem::UGameDataSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PlantTableObj(TEXT("/Game/GameFiles/GameData/DT_PlantData.DT_PlantData"));
	if (PlantTableObj.Succeeded())
	{
		PlantDataTable = PlantTableObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> LevelTableObj(TEXT("/Game/GameFiles/GameData/DT_LevelData.DT_LevelData"));
	if (LevelTableObj.Succeeded())
	{
		LevelDataTable = LevelTableObj.Object;
	}
}

void UGameDataSubsystem::GetPlantData(const int PlantIndex, FPlantData& PlantData, bool& bFoundPlant) const
{
    bFoundPlant = false;

	if (!PlantDataTable)
		return;

	const FName RowName = FName(*FString::FromInt(PlantIndex));
	static const FString Context = TEXT("PlantData Lookup");

	const FPlantData* FoundRow = PlantDataTable->FindRow<FPlantData>(RowName, Context);
	if (!FoundRow)
		return;

	PlantData = *FoundRow;
	bFoundPlant = true;
}


void UGameDataSubsystem::GetAllPlantData(TArray<FPlantData>& PlantDataArray) const
{
	PlantDataArray.Empty();

	if (!PlantDataTable)
		return;

	TArray<FName> RowNames = PlantDataTable->GetRowNames();
	PlantDataArray.Reserve(RowNames.Num());
	
	static const FString Context = TEXT("PlantData GetAll");

	for (const FName& RowName : RowNames)
	{
		if (const FPlantData* Row = PlantDataTable->FindRow<FPlantData>(RowName, Context))
			PlantDataArray.Add(*Row);
	}

}

void UGameDataSubsystem::GetLevelData(const int LevelIndex, FLevelData& LevelData, bool& bFoundLevel) const
{
	bFoundLevel = false;

	if (!LevelDataTable)
		return;

	const FName RowName = FName(*FString::Printf(TEXT("Level_%d"), LevelIndex + 1));
	static const FString Context = TEXT("LevelData Lookup");

	const FLevelData* FoundRow = LevelDataTable->FindRow<FLevelData>(RowName, Context);
	if (!FoundRow)
		return;

	LevelData = *FoundRow;
	bFoundLevel = true;
}

void UGameDataSubsystem::GetAllLevelData(TArray<FLevelData>& LevelDataArray) const
{
	LevelDataArray.Empty();

	if (!LevelDataTable)
		return;

	TArray<FName> RowNames = LevelDataTable->GetRowNames();
	LevelDataArray.Reserve(RowNames.Num());
	
	static const FString Context = TEXT("LevelData GetAll");

	for (const FName& RowName : RowNames)
	{
		if (const FLevelData* Row = LevelDataTable->FindRow<FLevelData>(RowName, Context))
			LevelDataArray.Add(*Row);
	}

}
