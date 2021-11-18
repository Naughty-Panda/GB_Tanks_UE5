// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TanksSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API UTanksSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float SavedFloat;
};
