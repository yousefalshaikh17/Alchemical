// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCompleteWidget.h"

void ULevelCompleteWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
	OnVisibilityChanged.AddDynamic(this, &ULevelCompleteWidget::VisibilityChanged);
}
