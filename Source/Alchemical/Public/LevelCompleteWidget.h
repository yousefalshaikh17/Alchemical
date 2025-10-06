// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelCompleteWidget.generated.h"

/**
 * Simple super class for Level Complete Widget made to expose properties to C++
 */
UCLASS()
class ALCHEMICAL_API ULevelCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool FinalLevelCompleted = false;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void VisibilityChanged(ESlateVisibility NewVisibility);
};
