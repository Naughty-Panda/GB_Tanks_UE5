// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankBase.h"

#include "Turret.generated.h"

UCLASS()
class GB_TANKS_UE5_API ATurret : public ATankBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 3000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingAccuracy = 10.0f;

	UPROPERTY()
	APawn* PlayerPawn;

private:
	FTimerHandle TargetingTimerHandle;

	//** Links to turret's assets */
	const FString BodyMeshAsset = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshAsset = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

public:
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void Targeting();
	bool IsPlayerInRange() const;
	bool CanFire() const;
	void RotateToPlayer();
};
