// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagerComponent.h"

#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "EquipmentComponent.h"
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
	if (!DraggedFrom || !DroppedTo)
	{
		return;
	}

	// Get both components: Inventory or Equipment.
	UInventoryComponent* FromComponent = DraggedFrom->ParentInventoryWidget->RepresentedComponent;
	UInventoryComponent* ToComponent = DroppedTo->ParentInventoryWidget->RepresentedComponent;

	if (!FromComponent || !ToComponent)
	{
		return;
	}

	FInventorySlotInfo FromSlot = DraggedFrom->GetItem();
	FInventorySlotInfo ToSlot = DroppedTo->GetItem();

	// Slot is empty. Nothing to do here.
	if (FromSlot.Count <= 0)
	{
		return;
	}

	FInventoryItemInfo* FromInfo = GetItemData(FromSlot.ID);
	FInventoryItemInfo* ToInfo = GetItemData(ToSlot.ID);

	if (!FromInfo || (!ToInfo && ToSlot.ID != NAME_None))
	{
		return;
	}

	const int32 MaxCount = ToComponent->GetMaxItemAmount(DroppedTo->IndexInInventory, *FromInfo);

	// Can't put anything.
	if (MaxCount == 0)
	{
		return;
	}

	// Can put any number of items.
	if (MaxCount < 0)
	{
		if (FromSlot.ID == ToSlot.ID)
		{
			FromSlot.Count += ToSlot.Count;
			ToSlot.Count = 0;
		}
	}

	if (MaxCount > 0)
	{
		// Check number of items we can add to new slot.
		const int32 NumItemsToAdd = FMath::Min(MaxCount, FromSlot.Count);

		ToSlot.Count = FromSlot.Count - NumItemsToAdd;
		ToSlot.ID = FromSlot.ID;
		ToInfo = FromInfo;

		FromSlot.Count = NumItemsToAdd;
	}

	FromComponent->SetItem(DraggedFrom->IndexInInventory, ToSlot);
	ToComponent->SetItem(DroppedTo->IndexInInventory, FromSlot);

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

void UInventoryManagerComponent::InitInventory(UInventoryComponent* InComponent)
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
	InventoryWidget->RepresentedComponent = LocalInventoryComponent;
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

void UInventoryManagerComponent::InitEquipment(UInventoryComponent* InComponent)
{
	ensure(InComponent);
	ensure(EquipmentWidgetClass);

	EquipmentWidget = CreateWidget<UInventoryWidget>(GetWorld(), EquipmentWidgetClass);
	EquipmentWidget->RepresentedComponent = InComponent;

	// TODO: Find a proper way to include Equipment System module...
	// if (UEquipmentComponent* EquipmentComponent = Cast<UEquipmentComponent>(EquipmentWidget->RepresentedComponent))
	// {
	// 	EquipmentComponent->OnItemEquipped.BindDynamic(this, &UInventoryManagerComponent::EquipItem);
	// 	EquipmentComponent->OnItemUnequipped.BindDynamic(this, &UInventoryManagerComponent::UnequipItem);
	// }

	EquipmentWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
	EquipmentWidget->AddToViewport();
}

void UInventoryManagerComponent::EquipItem(EEquipmentSlot EquipSlot, FName ItemID)
{
	if (FInventoryItemInfo* Data = GetItemData(ItemID))
	{
		if (UStaticMeshComponent* MeshComponent = GetEquipmentComponent(EquipSlot))
		{
			MeshComponent->SetStaticMesh(Data->Mesh.LoadSynchronous());
		}

		// If Item has stats we can apply them here.
		// Damage += Data->Damage;
		// Armor += Data->Armor;
		// e.t.c.
	}
}

void UInventoryManagerComponent::UnequipItem(EEquipmentSlot EquipSlot, FName ItemID)
{
	// Clear StaticMeshComponent.
	if (UStaticMeshComponent* MeshComponent = GetEquipmentComponent(EquipSlot))
	{
		MeshComponent->SetStaticMesh(nullptr);
	}

	// Clear stats if needed.
	if (FInventoryItemInfo* Data = GetItemData(ItemID))
	{
		// Damage -= Data->Damage;
		// Armor -= Data->Armor;
	}
}

UStaticMeshComponent* UInventoryManagerComponent::GetEquipmentComponent(EEquipmentSlot EquipSlot) const
{
	FName ComponentTag;

	switch (EquipSlot)
	{
	case EEquipmentSlot::ES_Head:
		ComponentTag = "BodyAttachment1";
		break;
	case EEquipmentSlot::ES_Legs:
		ComponentTag = "BodyAttachment2";
		break;
	default:
		return nullptr;
	}

	TArray<UActorComponent*> ActorComponents = GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), ComponentTag);
	return ActorComponents.IsValidIndex(0) ? Cast<UStaticMeshComponent>(ActorComponents[0]) : nullptr;
}
