// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class ALCHEMICAL_API APooledActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APooledActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pool")
	void PooledBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pool")
	void PooledEndPlay();

	void SetActiveState(bool bActive);

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pool")
	bool bIsActive = false;

	void ActivateFromPool();
	void DeactivateFromPool();

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void Deactivate();
};
