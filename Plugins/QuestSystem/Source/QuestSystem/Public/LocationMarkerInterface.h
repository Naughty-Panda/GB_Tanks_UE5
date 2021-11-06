// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "LocationMarkerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULocationMarkerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocationReached, AActor* LocationMarker, AActor* OverlappedActor);

class QUESTSYSTEM_API ILocationMarkerInterface
{
	GENERATED_BODY()

public:
	FOnLocationReached OnLocationReached;

public:
	virtual void NotifyOverlapWithActor(AActor* LocationMarker, AActor* OverlappedActor)
	{
		OnLocationReached.Broadcast(LocationMarker, OverlappedActor);
	}
};
