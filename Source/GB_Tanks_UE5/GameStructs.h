#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/UserDefinedEnum.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class EInputAxis : uint8
{
	AxisX = 0 UMETA(DisplayName = "X Axis"),
	AxisY = 1 UMETA(DisplayName = "Y Axis"),
	AxisZ = 2 UMETA(DisplayName = "Z Axis"),
};

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	ProjectileCannon = 0 UMETA(DisplayName = "Use projectiles"),
	TraceCannon = 1 UMETA(DisplayName = "Use traces"),
	FireCharge = 2 UMETA(DisplayName = "Use charge trace"),
};

UENUM(BlueprintType)
enum class ECannonFireMode : uint8
{
	Single = 0 UMETA(DisplayName = "Single shot"),
	Burst = 1 UMETA(DisplayName = "Burst fire"),
	Charge = 2 UMETA(DisplayName = "Requires charging a shot"),
};

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	WT_None,
	WT_MainMenu,
	WT_OptionsMenu,
	WT_GameOverMenu
};

USTRUCT(BlueprintType)
struct FCannonAmmo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo = MaxAmmo;

	FCannonAmmo& operator++()
	{
		CurrentAmmo = FMath::Clamp(++CurrentAmmo, 0, MaxAmmo);
		return *this;
	}

	FCannonAmmo operator++(int)
	{
		FCannonAmmo Out = *this;
		++*this;
		return Out;
	}

	FCannonAmmo& operator--()
	{
		CurrentAmmo = FMath::Clamp(--CurrentAmmo, 0, MaxAmmo);
		return *this;
	}

	FCannonAmmo operator--(int)
	{
		FCannonAmmo Out = *this;
		--*this;
		return Out;
	}

	bool FORCEINLINE HasAmmo() const
	{
		return CurrentAmmo > 0;
	}

	void FORCEINLINE AddAmmo(uint8 Amount)
	{
		CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, 0, MaxAmmo);
	}

	bool FORCEINLINE HasFullAmmo() const
	{
		return CurrentAmmo == MaxAmmo;
	}
};

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageValue = 0.0f;

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	AActor* DamageDealer;
};
