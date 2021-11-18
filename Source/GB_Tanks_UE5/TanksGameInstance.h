// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TanksGameInstance.generated.h"

class USaveManager;

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API UTanksGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USaveManager* SaveManager;
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintPure, meta=(WorldContext = WorldContextObject))
	static USaveManager* GetSaveManager(UObject* WorldContextObject);
};
