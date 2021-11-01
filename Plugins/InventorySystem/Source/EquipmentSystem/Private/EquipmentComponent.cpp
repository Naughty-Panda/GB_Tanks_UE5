// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentComponent.h"

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	EquipmentCharacter = GetOwner();
}

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	EquipmentSlots.Reserve(6);
	EquipmentSlots.Add(0, EEquipmentSlot::ES_Head);
	EquipmentSlots.Add(1, EEquipmentSlot::ES_Body);
	EquipmentSlots.Add(2, EEquipmentSlot::ES_LeftArm);
	EquipmentSlots.Add(3, EEquipmentSlot::ES_RightArm);
	EquipmentSlots.Add(4, EEquipmentSlot::ES_Legs);
	EquipmentSlots.Add(5, EEquipmentSlot::ES_Feet);
}

int32 UEquipmentComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
	if (const FInventorySlotInfo* InfoPtr = Items.Find(SlotIndex))
	{
		if (InfoPtr->ID != NAME_None)
		{
			if (Item.ItemType != EItemType::IT_Equipment)
			{
				if (!EquipmentSlots.Contains(SlotIndex))
				{
					if (EquipmentSlots.FindChecked(SlotIndex) != Item.EquipmentSlot)
					{
						// Can't drop equipment here.
						return 0;
					}
				}
			}
		}
	}

	// Can equip 1 item.
	return 1;
}

void UEquipmentComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
	FInventorySlotInfo* Equipment = GetItem(SlotIndex);
	EEquipmentSlot EquipmentSlot = EquipmentSlots.Contains(SlotIndex) ? EquipmentSlots.FindChecked(SlotIndex) : EEquipmentSlot::ES_None;

	if (Equipment)
	{
		OnItemUnequipped.ExecuteIfBound(EquipmentSlot, Equipment->ID);
	}

	Super::SetItem(SlotIndex, Item);

	std::ignore = OnItemEquipped.ExecuteIfBound(EquipmentSlot, Item.ID);
}
