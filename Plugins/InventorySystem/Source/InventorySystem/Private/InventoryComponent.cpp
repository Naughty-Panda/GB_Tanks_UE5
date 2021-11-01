// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick Tick, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);
}

FInventorySlotInfo* UInventoryComponent::GetItem(int32 SlotIndex)
{
	return Items.Find(SlotIndex);
}

const TMap<int32, FInventorySlotInfo>& UInventoryComponent::GetItems()
{
	return Items;
}

int32 UInventoryComponent::GetItemsNum() const
{
	return Items.Num();
}

int32 UInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
	// Check if the Slot already contains something.
	if (const FInventorySlotInfo* InfoPtr = Items.Find(SlotIndex))
	{
		// Check if the Slot already contain the same item we want to drop in it.
		if (InfoPtr->ID != Item.ID)
		{
			// Can't drop an item.
			return 0;
		}
	}

	// Can drop any number of same items.
	return -1;
}

void UInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& InItem)
{
	ClearItem(SlotIndex);
	Items.Add(SlotIndex, InItem);
}

void UInventoryComponent::ClearItem(int32 SlotIndex)
{
	Items.Remove(SlotIndex);
}
