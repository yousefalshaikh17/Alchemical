// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"

#include "ItemInstance.generated.h"

/**
 * Structure to represent item
 */
USTRUCT(BlueprintType)
struct FItemInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemType Type = EItemType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int PlantIndex = -1;
};
