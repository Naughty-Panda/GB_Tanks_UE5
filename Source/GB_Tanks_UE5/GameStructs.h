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
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use trace"),
	FireCharge = 2 UMETA(DisplayName = "Use charge trace"),
};

UENUM(BlueprintType)
enum class ECannonFireMode : uint8
{
	Single = 0 UMETA(DisplayName = "Single shot"),
	Burst = 1 UMETA(DisplayName = "Burst fire"),
	Charge = 2 UMETA(DisplayName = "Requires charging a shot"),
};
