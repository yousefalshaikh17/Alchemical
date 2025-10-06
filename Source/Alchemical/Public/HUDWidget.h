// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMICAL_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateScore(const int NewScore);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateHealth(const int NewHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateLevelData(const int LevelDisplayNumber, const int NewRequiredScore);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int MaxHeartCount = 3;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(BindWidget))
	UPanelWidget* HeartContainer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTexture2D* HeartTexture;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FVector2D HeartImageSize = FVector2D(64, 64);

	UPROPERTY()
	TArray<UImage*> HeartImages;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(BindWidget))
	UTextBlock* LevelNumberLabel;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(BindWidget))
	UTextBlock* RequiredScoreLabel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(BindWidget))
	UTextBlock* ScoreLabel;
};
