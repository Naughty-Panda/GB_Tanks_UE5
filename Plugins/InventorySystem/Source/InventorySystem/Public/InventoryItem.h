// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CompositeDataTable.h"

#include "InventoryItem.generated.h"

/**
 * 
 */

UENUM(BlueprintType, Blueprintable)
enum class EItemType : uint8
{
	IT_None,
	IT_Currency,
	IT_Equipment
};

UENUM(BlueprintType, Blueprintable)
enum class EItemRarity : uint8
{
	IR_Common,
	IR_Rare,
	IR_Magic,
	IR_Epic,
	IR_Legendary
};

UENUM(BlueprintType, Blueprintable)
enum class EEquipmentSlot : uint8
{
	ES_None,
	ES_Head,
	ES_Body,
	ES_LeftArm,
	ES_RightArm,
	ES_Legs,
	ES_Feet
};

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FInventoryItemInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	 * System info.
	 * Controlled by Parent DataTable RowName.
	 */
	UPROPERTY(VisibleAnywhere, Category="ID")
	FName ID;

	/** Item name and description */
	UPROPERTY(EditAnywhere, Category="General")
	FText Name;

	UPROPERTY(EditAnywhere, Category="General")
	FText Description;

	/** Enum types */
	UPROPERTY(EditAnywhere, Category="Types")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category="Types")
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, Category="Types")
	EEquipmentSlot EquipmentSlot;

	/** Item visuals */
	UPROPERTY(EditAnywhere, Category="Visuals")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category="Visuals")
	TSoftObjectPtr<UStaticMesh> Mesh;

	/** Item stats */
	UPROPERTY(EditAnywhere, Category="Stats")
	int32 Damage;

	UPROPERTY(EditAnywhere, Category="Stats")
	int32 Armor;

	UPROPERTY(EditAnywhere, Category="Stats")
	int32 Intelligence;

protected:
	/** Overriding to setup our ID. */
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
};

USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	FName ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	int32 Count;
};
