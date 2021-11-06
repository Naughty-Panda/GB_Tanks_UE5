// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CollectableItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCollectableItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemCollected, int32 ItemID, int32 ItemCount);

class QUESTSYSTEM_API ICollectableItemInterface
{
	GENERATED_BODY()

public:
	FOnItemCollected OnItemCollected;

public:
	virtual void NotifyItemCollected(int32 ItemID, int32 ItemCount)
	{
		OnItemCollected.Broadcast(ItemID, ItemCount);
	}
};
