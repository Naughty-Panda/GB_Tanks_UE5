// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

void ATurret::Destroyed()
{
	Super::Destroyed();

	if (GetPrimaryCannon())
	{
		GetPrimaryCannon()->Destroy();
	}
}

void ATurret::Targeting()
{
	if (IsPlayerInRange())
	{
		RotateToPlayer();
		ACannon* Cannon = GetPrimaryCannon();
		if (CanFire() && Cannon && Cannon->IsReadyToFire())
		{
			Cannon->Fire(ECannonFireMode::Single);
		}
	}
}

bool ATurret::IsPlayerInRange() const
{
	return FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation()) < TargetingRange;
}

bool ATurret::CanFire() const
{
	const FVector TargetingDirection = TurretMesh->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Normalize();
	const float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
	return AimAngle < TargetingAccuracy;
}

void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TurrerRotInterpSpeed));
}
