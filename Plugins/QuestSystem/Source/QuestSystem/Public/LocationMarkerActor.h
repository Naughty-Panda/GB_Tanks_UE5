// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocationMarkerInterface.h"
#include "GameFramework/Actor.h"

#include "LocationMarkerActor.generated.h"

UCLASS()
class QUESTSYSTEM_API ALocationMarkerActor : public AActor, public ILocationMarkerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALocationMarkerActor();

public:
	UFUNCTION(BlueprintCallable)
	void OnOverlappedByActor(AActor* OverlappingActor)
	{
		NotifyOverlapWithActor(this, OverlappingActor);
	}
};
