// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBot.h"

ATankBot::ATankBot()
{
	PrimaryActorTick.bCanEverTick = true;

	// Allows to edit AI-related settings
	bIsAIControlled = true;
}

const TArray<FVector>& ATankBot::GetPatrollingPoints() const
{
	return PatrollingPoints;
}

float ATankBot::GetMovementAccuracy() const
{
	return MovementAccuracy;
}
