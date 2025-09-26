// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "ElementIconWidget.h"
#include "ElementTypesDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMICAL_API UElementTypesDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0"))
	int MaxElementCount = 1;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateElementDisplay(TSet<EElement> NewElements);

	UFUNCTION(BlueprintCallable)
	void ClearElementDisplay();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPanelWidget* Container;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UElementIconWidget> ElementWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UElementIconWidget*> ElementIconWidgets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int DisplayedElementsCount = 0;

	UFUNCTION(BlueprintCallable)
	void AddElement(const EElement Element);
};
