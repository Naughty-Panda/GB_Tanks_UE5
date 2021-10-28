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

/**
 * Inventory Manager Actor Component.
 * Used to interact with inventory component.
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
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

	void Init(UInventoryComponent* InComponent);
	FORCEINLINE FInventoryItemInfo* GetItemData(FName ItemID) const;
};
