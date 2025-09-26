// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Element.h"
#include "ElementIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMICAL_API UElementIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateElement(EElement NewElement);
};
