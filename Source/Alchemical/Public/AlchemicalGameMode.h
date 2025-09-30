// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlchemicalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMICAL_API AAlchemicalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RegisterResettableActor(AActor* Actor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnregisterResettableActor(const AActor* Actor);
	
	virtual void ResetLevel() override;

protected:
	TSet<TWeakObjectPtr<AActor>> ResettableActors;
};
