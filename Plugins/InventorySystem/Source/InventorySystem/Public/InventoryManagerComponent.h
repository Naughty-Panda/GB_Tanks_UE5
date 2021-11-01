// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InventoryManagerComponent.generated.h"

class UInventoryCellWidget;
class UInventoryWidget;
class UDataTable;
class UInventoryComponent;
struct FInventoryItemInfo;
enum class EEquipmentSlot : uint8;

/**
 * Inventory Manager Actor Component.
 * Used to interact with inventory component.
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Inventory part */
	UPROPERTY()
	UInventoryComponent* LocalInventoryComponent;

	UPROPERTY(EditAnywhere, Category="Inventory")
	UDataTable* InventoryItemsData;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 MinInventorySize = 20;

	/** Inventory part */
	UPROPERTY()
	UInventoryWidget* EquipmentWidget;

	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<UInventoryWidget> EquipmentWidgetClass;

public:
	// Sets default values for this component's properties
	UInventoryManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo) const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitInventory(UInventoryComponent* InComponent);
	FORCEINLINE FInventoryItemInfo* GetItemData(FName ItemID) const;

	// Equipment Component Initialization
	void InitEquipment(UInventoryComponent* InComponent);

	// Equipment methods
	void EquipItem(EEquipmentSlot EquipSlot, FName ItemID);
	void UnequipItem(EEquipmentSlot EquipSlot, FName ItemID);

	UStaticMeshComponent* GetEquipmentComponent(EEquipmentSlot EquipSlot) const;
};
