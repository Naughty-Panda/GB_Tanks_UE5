// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"

#include "Cannon.generated.h"

class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class GB_TANKS_UE5_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	float FireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	float FireRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	float FireDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	ECannonType CannonType = ECannonType::FireTrace;

	//** Available ammo. TODO: Move to Actor Component class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire settings")
	int32 AmmoCount = 10;

	FTimerHandle ReloadTimerHandle;
	FTimerHandle BurstFireTimerHandle;

	bool bReadyToFire = true;
public:
	// Sets default values for this actor's properties
	ACannon();

	void Fire();
	void FireSpecial();
	void BurstFire();
	bool IsReadyToFire() const { return bReadyToFire && AmmoCount; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// TODO: Move to Actor Component class
	void Reload() { bReadyToFire = true; }
	void ConsumeAmmo(int32 Count = 1);
};
