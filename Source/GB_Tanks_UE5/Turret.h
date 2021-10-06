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

	void Targeting();
};
