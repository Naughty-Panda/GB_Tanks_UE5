// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryCellWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryCellWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Initialize our bit fields here, cause c++17.
	bIsDraggable = true;
	bHasItem = false;
}

bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& InItem, const FInventoryItemInfo& InInfo)
{
	if (bHasItem)
	{
		return false;
	}

	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(InInfo.Icon.Get());
		ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(InItem.Count)));
		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	bHasItem = true;
	StoredItem = InItem;

	return true;
}

void UInventoryCellWidget::Clear()
{
	if (bHasItem)
	{
		if (ItemImage)
		{
			ItemImage->SetBrush(FSlateBrush());
			ItemImage->SetVisibility(ESlateVisibility::Hidden);
		}

		if (CountText)
		{
			CountText->SetText(FText());
			CountText->SetVisibility(ESlateVisibility::Hidden);
		}

		bHasItem = false;
		StoredItem = FInventorySlotInfo();
	}
}

FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDraggable && bHasItem)
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}

	return FReply::Handled();
}

void UInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

	if (OutOperation)
	{
		OutOperation->DefaultDragVisual = this;
	}
}

bool UInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryCellWidget* OtherCell = InOperation ? Cast<UInventoryCellWidget>(InOperation->DefaultDragVisual) : nullptr;

	if (OtherCell && OtherCell != this)
	{
		OnItemDrop.Broadcast(OtherCell, this);
		return true;
	}

	return false;
}
