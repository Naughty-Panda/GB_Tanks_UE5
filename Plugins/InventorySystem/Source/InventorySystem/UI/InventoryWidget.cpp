// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::Init(int32 NumSlots)
{
	if (ItemCellsGrid)
	{
		ItemCellsGrid->ClearChildren();
		CellWidgets.Reserve(NumSlots);

		for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
		{
			if (UInventoryCellWidget* NewCell = CreateCellWidget())
			{
				NewCell->IndexInInventory = SlotIndex;
				// Row = SlotIndex / ItemsInRow, Column = SlotIndex % ItemsInRow
				ItemCellsGrid->AddChildToUniformGrid(NewCell, SlotIndex / ItemsInRow, SlotIndex % ItemsInRow);
			}
		}
	}
}

bool UInventoryWidget::AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 SlotPosition)
{
	// Check if we can add Gold in the GoldCell first
	if (ItemInfo.ItemType == EItemType::IT_Currency)
	{
		if (GoldCell)
		{
			// TODO: Add a check in widget's AddItem() to allow to add gold multiple times.
			// It'll work only the firs time we adding gold.
			GoldCell->AddItem(Item, ItemInfo);
		}
	}

	if (ItemCellsGrid)
	{
		// Find the first Cell in CellWidgets Array with SlotPosition that we need.
		// FindByPredicate() returns a pointer to fist item in TArray that matches lambda expression.
		UInventoryCellWidget** WidgetToAddItemPtr = CellWidgets.FindByPredicate([SlotPosition](UInventoryCellWidget* Cell)
		{
			return Cell && Cell->IndexInInventory == SlotPosition;
		});

		// If Cell with needed index is found, we will get its pointer.
		UInventoryCellWidget* WidgetToAddItem = nullptr;
		if (WidgetToAddItemPtr)
		{
			WidgetToAddItem = *WidgetToAddItemPtr;
		}
		else
		{
			// If we don't need a particular Cell, we'll use first empty Cell
			for (UInventoryCellWidget* Cell : CellWidgets)
			{
				if (!Cell->HasItem())
				{
					WidgetToAddItem = Cell;
					break;
				}
			}
		}

		if (WidgetToAddItem)
		{
			return WidgetToAddItem->AddItem(Item, ItemInfo);
		}
	}

	return false;
}

UInventoryCellWidget* UInventoryWidget::CreateCellWidget()
{
	if (CellWidgetClass)
	{
		// Creating new Cell Widget and binding its delegate.
		UInventoryCellWidget* NewCell = CreateWidget<UInventoryCellWidget>(this, CellWidgetClass);
		NewCell->OnItemDrop.AddUObject(this, &UInventoryWidget::OnItemDropped);

		CellWidgets.Add(NewCell);
		return NewCell;
	}

	return nullptr;
}

void UInventoryWidget::OnItemDropped(UInventoryCellWidget* FromCell, UInventoryCellWidget* ToCell) const
{
	OnItemDrop.Broadcast(FromCell, ToCell);
}
