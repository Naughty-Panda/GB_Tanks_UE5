// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Allows to edit AI-related settings
	bIsAIControlled = true;

	if (UStaticMesh* TempBodyMesh = LoadObject<UStaticMesh>(this, *BodyMeshAsset))
	{
		BodyMesh->SetStaticMesh(TempBodyMesh);
	}

	if (UStaticMesh* TempTurretMesh = LoadObject<UStaticMesh>(this, *TurretMeshAsset))
	{
		TurretMesh->SetStaticMesh(TempTurretMesh);
	}
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	// Cannon spawn handled by parent class

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, -1.0f);
}

void ATurret::Targeting()
{
	LockOnTarget(PlayerPawn);
}
