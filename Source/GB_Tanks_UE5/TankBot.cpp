// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBot.h"

ATankBot::ATankBot()
{
	PrimaryActorTick.bCanEverTick = true;

	// Allows to edit AI-related settings
	bIsAIControlled = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

const TArray<ATargetPoint*>& ATankBot::GetPatrollingPoints() const
{
	return PatrollingPoints;
}

float ATankBot::GetMovementAccuracy() const
{
	return MovementAccuracy;
}

void ATankBot::SetPatrollingPoints(const TArray<ATargetPoint*>& NewPoints)
{
	PatrollingPoints = NewPoints;
}
