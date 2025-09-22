// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"

class APooledActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMICAL_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorPool();

protected:
	UPROPERTY()
	TArray<APooledActor*> Pool;


	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializePool();
	APooledActor* CreatePoolActor();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	bool bIsDynamicallySized;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool", meta = (ClampMin = "0"))
	int PoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	TSubclassOf<class APooledActor>  PoolActorClass;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	APooledActor* RequestActor(const FTransform& SpawnTransform);
};
