// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectilePhysicsComponent.h"

// Sets default values for this component's properties
UProjectilePhysicsComponent::UProjectilePhysicsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

TArray<FVector> UProjectilePhysicsComponent::GenerateTrajectory(const FVector& StartPos, const FVector& Velocity,
                                                     float MaxTime, float TimeStep, float MinZVal /*= 0.f */)
{
	TArray<FVector> Trajectory;
	FVector GravityVector(0.f, 0.f, Gravity);

	for (float Time = 0.f; Time < MaxTime; Time += TimeStep)
	{
		FVector Position = StartPos + (Velocity * Time) + (Gravity * Time * Time / 2);

		if (Position.Z < MinZVal)
		{
			break;
		}

		Trajectory.Add(Position);
	}

	return MoveTemp(Trajectory);
}
