// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagerComponent.h"

#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "InventorySystem/UI/InventoryWidget.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryManagerComponent::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo) const
{
	if (!DraggedFrom || !DroppedTo || !LocalInventoryComponent)
	{
		return;
	}

	// Swap items
	const FInventorySlotInfo FromSlot = DraggedFrom->GetItem();
	const FInventorySlotInfo ToSlot = DroppedTo->GetItem();

	LocalInventoryComponent->SetItem(DraggedFrom->IndexInInventory, ToSlot);
	LocalInventoryComponent->SetItem(DroppedTo->IndexInInventory, FromSlot);

	FInventoryItemInfo* FromInfo = GetItemData(FromSlot.ID);
	FInventoryItemInfo* ToInfo = GetItemData(ToSlot.ID);

	if (!FromInfo || (!ToInfo && ToSlot.ID != NAME_None))
	{
		return;
	}

	DraggedFrom->Clear();
	if (ToInfo)
	{
		DraggedFrom->AddItem(ToSlot, *ToInfo);
	}

	DroppedTo->Clear();
	DroppedTo->AddItem(FromSlot, *FromInfo);
}

// Called every frame
void UInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryManagerComponent::Init(UInventoryComponent* InComponent)
{
	ensure(InComponent);
	ensure(InventoryItemsData);
	ensure(InventoryWidgetClass);

	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromViewport();
	}

	LocalInventoryComponent = InComponent;
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget->AddToViewport();
	InventoryWidget->Init(FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));
	InventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);

	// for(const auto& [Index, Item] : LocalInventoryComponent->GetItems()) -- c++17
	for (const auto& Item : LocalInventoryComponent->GetItems())
	{
		if (FInventoryItemInfo* Data = GetItemData(Item.Value.ID))
		{
			std::ignore = Data->Icon.LoadSynchronous();
			InventoryWidget->AddItem(Item.Value, *Data, Item.Key);
		}
	}
}

FInventoryItemInfo* UInventoryManagerComponent::GetItemData(FName ItemID) const
{
	return InventoryItemsData ? InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, "") : nullptr;
}
