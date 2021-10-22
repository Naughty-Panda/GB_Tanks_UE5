// Fill out your copyright notice in the Description page of Project Settings.

#include "DraggableTankWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TanksDragDropOperation.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UDraggableTankWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (TankNameTextBlock)
	{
		TankNameTextBlock->SetText(TankName);
	}
}

FReply UDraggableTankWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Handled();
}

void UDraggableTankWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (UTanksDragDropOperation* TanksDragDropOperation = Cast<UTanksDragDropOperation>(
		UWidgetBlueprintLibrary::CreateDragDropOperation(UTanksDragDropOperation::StaticClass())))
	{
		TanksDragDropOperation->TankName = TankName;
		TanksDragDropOperation->DefaultDragVisual = this;

		constexpr FLinearColor DraggedColor(1.f, 1.f, 1.f, 0.6f);
		SetColorAndOpacity(DraggedColor);
		SetVisibility(ESlateVisibility::HitTestInvisible);

		TanksDragDropOperation->OnDragCancelled.AddDynamic(this, &UDraggableTankWidget::OnDragCancelledHandle);
		OutOperation = TanksDragDropOperation;
	}
}

/** New Implementation */
bool UDraggableTankWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UVerticalBox* VerticalBox = Cast<UVerticalBox>(GetParent());
	const UTanksDragDropOperation* TanksDragDropOperation = Cast<UTanksDragDropOperation>(InOperation);


	if (VerticalBox && TanksDragDropOperation)
	{
		// Get pointer to the draggable widget from the operation.
		const TObjectPtr<UWidget> SelectedWidget = TanksDragDropOperation->DefaultDragVisual;

		if (SelectedWidget && SelectedWidget != this)
		{
			// Check if this widget is located in our vertical box.
			const int32 ThisIndex = VerticalBox->GetChildIndex(SelectedWidget);

			if (ThisIndex >= 0)
			{
				// TODO: This members of FGeometry is deprecated, so we need to find a better way to calculate desired index.
				// Calculate desired index in vertical box based on new widget position.
				const int32 DesiredIndex = InGeometry.Position.Y / InGeometry.Size.Y;

				// Remove this widget from parent.
				VerticalBox->RemoveChildAt(ThisIndex);

				// Get all other elements in the vertical box.
				auto ExistingWidgets = VerticalBox->GetAllChildren();
				VerticalBox->ClearChildren();

				// Insert previously removed widget back to its desired position in array.
				ExistingWidgets.Insert(SelectedWidget.Get(), DesiredIndex);

				for (const auto& Widget : ExistingWidgets)
				{
					// Populating vertical box from widget array and aligning newly added widgets.
					VerticalBox->AddChildToVerticalBox(Widget)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				}

				return true;
			}
		}
	}

	return false;
}

/** Default Implementation
bool UDraggableTankWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UVerticalBox* VerticalBox = Cast<UVerticalBox>(GetParent());
	const UTanksDragDropOperation* TanksDragDropOperation = Cast<UTanksDragDropOperation>(InOperation);

	if (VerticalBox && TanksDragDropOperation)
	{
		if (TanksDragDropOperation->DefaultDragVisual && TanksDragDropOperation->DefaultDragVisual != this)
		{
			// Check if this widget is located in our vertical box.
			const int32 ThisIndex = VerticalBox->GetChildIndex(TanksDragDropOperation->DefaultDragVisual);

			if (ThisIndex >= 0)
			{
				// Replacing this widget in vertical box.
				VerticalBox->RemoveChildAt(ThisIndex);

				const auto ExistingWidgets = VerticalBox->GetAllChildren();
				VerticalBox->ClearChildren();

				for (const auto& Widget : ExistingWidgets)
				{
					// Inserting all other widgets and aligning them.
					VerticalBox->AddChildToVerticalBox(Widget)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);

					// Inserting dragged widget above this widget and align it.
					if (Widget == this)
					{
						VerticalBox->AddChildToVerticalBox(TanksDragDropOperation->DefaultDragVisual)->SetHorizontalAlignment(
							EHorizontalAlignment::HAlign_Center);
					}
				}

				return true;
			}
		}
	}

	return false;
} */

bool UDraggableTankWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation)
	{
		InOperation->OnDragCancelled.Broadcast(InOperation);
		return true;
	}

	return false;
}

void UDraggableTankWidget::OnDragCancelledHandle(UDragDropOperation* InOperation)
{
	// Return this Widget color and visibility back to normal.
	SetColorAndOpacity(FLinearColor::White);
	SetVisibility(ESlateVisibility::Visible);
}
