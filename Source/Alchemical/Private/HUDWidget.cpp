// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	HeartImages.Reserve(MaxHeartCount);
	
	FSlateBrush Brush;
	Brush.SetResourceObject(HeartTexture);
	Brush.SetImageSize(HeartImageSize);
	
	for (int i = 0; i < MaxHeartCount; i++)
	{
		UImage* HeartImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		HeartImage->SetBrush(Brush);
		HeartContainer->AddChild(HeartImage);
		SetVisibility(ESlateVisibility::Hidden);
		HeartImages.Add(HeartImage);
	}
}

void UHUDWidget::UpdateLevelData_Implementation(const int LevelDisplayNumber, const int NewRequiredScore)
{
	LevelNumberLabel->SetText(FText::FromString(FString::FromInt(LevelDisplayNumber)));
	RequiredScoreLabel->SetText(FText::FromString(FString::FromInt(NewRequiredScore)));
}

void UHUDWidget::UpdateHealth_Implementation(const int NewHealth)
{
	for (int i = 0; i < HeartImages.Num(); i++)
	{
		HeartImages[i]->SetVisibility( (i < NewHealth) ? ESlateVisibility::Visible : ESlateVisibility::Hidden );
	}
}

void UHUDWidget::UpdateScore_Implementation(const int NewScore)
{
	ScoreLabel->SetText(FText::FromString(FString::FromInt(NewScore)));
}


