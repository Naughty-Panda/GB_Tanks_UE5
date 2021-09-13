// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsProjectile.h"
#include "Components/ProjectilePhysicsComponent.h"

#include <Kismet/KismetMathLibrary.h>
#include <DrawDebugHelpers.h>
#include <Particles/ParticleSystemComponent.h>

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UProjectilePhysicsComponent>(TEXT("Physics component"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
	MoveVector = GetActorForwardVector() * TrajectorySimSpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimMaxTime, TrajectorySimTimeStep);
	if (bShowTrajectory)
	{
		for (const auto& Position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), Position, 5.f, 8, FColor::Purple, true, 1.f, 0, 2.f);
		}
	}

	TrajectoryPointIndex = 0;

	Super::Start();
}

void APhysicsProjectile::Move()
{
	const FVector CurrentLocation = GetActorLocation();
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - CurrentLocation;
	CurrentMoveVector.Normalize();

	const FVector NewLocation = CurrentLocation + CurrentMoveVector * MoveSpeed * MoveRate;

	SetActorLocation(NewLocation);

	if (FVector::Distance(NewLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MovementAccuracy)
	{
		++TrajectoryPointIndex;

		if (TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			Destroy();
		}
		else
		{
			const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, CurrentTrajectory[TrajectoryPointIndex]);
			SetActorRotation(NewRotation);
		}
	}
	// Super::Move();
}
