// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"

#include "AmmoBox.generated.h"

UCLASS()
class GB_TANKS_UE5_API AAmmoBox : public ACollectableItem
{
	GENERATED_BODY()

public:
	//** Amount of ammo to be added to player's Tank */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	uint8 AmmoCount;

	AAmmoBox();

protected:
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                            bool bFromSweep, const FHitResult& HitResult) override;
};
