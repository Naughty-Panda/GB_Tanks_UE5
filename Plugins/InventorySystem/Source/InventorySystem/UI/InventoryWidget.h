// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryCellWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

struct FInventorySlotInfo;
struct FInventoryItemInfo;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/** Main Items Grid */
	UPROPERTY(meta = (BindWidgetOptional))
	UUniformGridPanel* ItemCellsGrid;

	/** Optional inventory cell for Gold */
	UPROPERTY(meta = (BindWidgetOptional))
	UInventoryCellWidget* GoldCell;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInRow = 5;

	/** Default class for Cell widgets */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;

	/** Array of created Cell widgets */
	UPROPERTY(BlueprintReadWrite)
	TArray<UInventoryCellWidget*> CellWidgets;

public:
	/** Item Drop Delegate */
	FOnItemDrop OnItemDrop;

public:
	virtual void NativeConstruct() override;

	void Init(int32 NumSlots);
	bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 SlotPosition = -1);

protected:
	UInventoryCellWidget* CreateCellWidget();
	void OnItemDropped(UInventoryCellWidget* FromCell, UInventoryCellWidget* ToCell) const;
};
