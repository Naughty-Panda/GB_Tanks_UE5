// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

/**
 * Inventory Actor Component.
 * Used as simple storage for items.
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TMap<int32, FInventorySlotInfo> Items;

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FInventorySlotInfo* GetItem(int32 SlotIndex);
	const TMap<int32, FInventorySlotInfo>& GetItems();
	int32 GetItemsNum() const;
	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& InItem);
	void ClearItem(int32 SlotIndex);
};
