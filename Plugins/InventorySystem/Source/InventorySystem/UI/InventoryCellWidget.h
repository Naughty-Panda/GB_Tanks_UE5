// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "InventoryManagerComponent.h"
#include "Blueprint/UserWidget.h"

#include "InventoryCellWidget.generated.h"

class UImage;
class UTextBlock;
class UInventoryWidget;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, UInventoryCellWidget* From, UInventoryCellWidget* To)
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryWidget* ParentInventoryWidget;

	UPROPERTY(EditAnywhere)
	int32 IndexInInventory = -1;

protected:
	UPROPERTY()
	FInventorySlotInfo StoredItem;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CountText;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsDraggable :1;

	uint8 bHasItem :1;

public:
	FOnItemDrop OnItemDrop;

public:
	virtual void NativeOnInitialized() override;
	
	FORCEINLINE bool HasItem() const { return bHasItem; }
	bool AddItem(const FInventorySlotInfo& InItem, const FInventoryItemInfo& InInfo);
	const FInventorySlotInfo& GetItem() const { return StoredItem; }
	void Clear();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
