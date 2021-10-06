// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "Cannon.h"

#include "WeaponBox.generated.h"

UCLASS()
class GB_TANKS_UE5_API AWeaponBox : public ACollectableItem
{
	GENERATED_BODY()

public:
	//** Cannon class for the tank */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ACannon> Cannon;

	AWeaponBox();

protected:
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                            AActor* OtherActor,
	                            UPrimitiveComponent* OtherComponent,
	                            int32 OtherBodyIndex,
	                            bool bFromSweep,
	                            const FHitResult& HitResult) override;
};
