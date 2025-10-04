// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlchemicalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMICAL_API AAlchemicalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnScoreChanged(int NewScore);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHealthChanged(int NewHealth);
};
