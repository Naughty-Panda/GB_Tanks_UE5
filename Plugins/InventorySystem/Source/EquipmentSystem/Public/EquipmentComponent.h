// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"

#include "EquipmentComponent.generated.h"


/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnItemEquipped, EEquipmentSlot, EquipSlot, FName, ID);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnItemUnequipped, EEquipmentSlot, EquipSlot, FName, ID);

UCLASS()
class EQUIPMENTSYSTEM_API UEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()

protected:
	/** Map of all available Equipment Slots for a Character */
	UPROPERTY(EditAnywhere)
	TMap<int32, EEquipmentSlot> EquipmentSlots;

	/** Owner of this slots */
	UPROPERTY()
	AActor* EquipmentCharacter;

public:
	/** Delegates */
	FOnItemEquipped OnItemEquipped;
	FOnItemUnequipped OnItemUnequipped;

protected:
	virtual void BeginPlay() override;

public:
	UEquipmentComponent();
	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const override;
	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item) override;
};
