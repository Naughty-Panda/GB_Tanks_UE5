// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InventoryItem.h"

#include "TanksSaveGame.generated.h"

enum class ECannonType : uint8;

/**
 * Base Tank Save Data
 */

USTRUCT(BlueprintType)
struct FTankSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHealth = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECannonType CannonType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CannonMaxAmmo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CannonCurrentAmmo = 0;
};

/**
 * Player Tank Save Data
 */

USTRUCT(BlueprintType)
struct FPlayerTankSaveData : public FTankSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECannonType SecondaryCannonType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SecondaryCannonMaxAmmo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SecondaryCannonCurrentAmmo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, FInventorySlotInfo> InventoryItems;

	FPlayerTankSaveData& operator=(FTankSaveData&& Other) noexcept
	{
		Location = MoveTemp(Other.Location);
		Rotation = MoveTemp(Other.Rotation);
		CurrentHealth = MoveTemp(Other.CurrentHealth);
		CannonType = MoveTemp(Other.CannonType);
		CannonMaxAmmo = MoveTemp(Other.CannonMaxAmmo);
		CannonCurrentAmmo = MoveTemp(Other.CannonCurrentAmmo);

		return *this;
	}
};

/**
 *
 */

UCLASS()
class GB_TANKS_UE5_API UTanksSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString LevelName;

	UPROPERTY(BlueprintReadWrite)
	FPlayerTankSaveData PlayerSaveData;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTankSaveData> AISaveData;
};
