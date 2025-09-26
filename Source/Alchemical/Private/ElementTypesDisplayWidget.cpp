#include "ElementTypesDisplayWidget.h"

void UElementTypesDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (MaxElementCount < 1) return;

	if (!ElementWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ElementWidgetClass is null!"));
		return;
	}


	// Initialize element display array
	ElementIconWidgets.Reserve(MaxElementCount);
	for (int i = 0; i < MaxElementCount; i++)
	{
		UElementIconWidget* ElementWidget = CreateWidget<UElementIconWidget>(GetWorld(), ElementWidgetClass);
		ElementIconWidgets.Add(ElementWidget);
	}
}

void UElementTypesDisplayWidget::AddElement(const EElement Element)
{
	if (!Container) return;

	if (!ElementIconWidgets.IsValidIndex(DisplayedElementsCount)) return;

	UElementIconWidget* ElementWidget = ElementIconWidgets[DisplayedElementsCount];
	ElementWidget->UpdateElement(Element);
	Container->AddChild(ElementWidget);
	DisplayedElementsCount++;
}

void UElementTypesDisplayWidget::ClearElementDisplay()
{
	for (int i = 0; i < DisplayedElementsCount; i++)
	{
		UElementIconWidget* ElementWidget = ElementIconWidgets[i];
		Container->RemoveChild(ElementWidget);
	}
	DisplayedElementsCount = 0;
}

void UElementTypesDisplayWidget::UpdateElementDisplay(TSet<EElement> NewElements)
{
	ClearElementDisplay();

	for (const EElement Element : NewElements)
	{
		AddElement(Element);
	}
}
