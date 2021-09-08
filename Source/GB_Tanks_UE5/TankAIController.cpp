// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "DrawDebugHelpers.h"
#include "TankBot.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	TankBot = Cast<ATankBot>(GetPawn());
	if (!TankBot)
	{
		return;
	}

	MovementAccuracy = TankBot->GetMovementAccuracy();
	TArray<FVector> BotPoints = TankBot->GetPatrollingPoints();
	const FVector CurrentLocation = TankBot->GetActorLocation();

	for (const auto& Point : BotPoints)
	{
		const FVector NewPoint = Point + CurrentLocation;
		PatrollingPoints.Add(NewPoint);
	}

	CurrentPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Movement tick
	if (CurrentPointIndex == INDEX_NONE)
	{
		TankBot->MoveForward(0.f);
		return;
	}

	TankBot->MoveForward(1.f);

	// Rotation tick
	const float RotationValue = CalculateMovementRotation();
	TankBot->Rotate(RotationValue);

	// Targeting tick
	TankBot->LockOnTarget(PlayerPawn);
}

float ATankAIController::CalculateMovementRotation()
{
	const FVector CurrentPoint = PatrollingPoints[CurrentPointIndex];
	const FVector PawnLocation = TankBot->GetActorLocation();

	if (FVector::Distance(PawnLocation, CurrentPoint) < MovementAccuracy)
	{
		++CurrentPointIndex;
		if (CurrentPointIndex >= PatrollingPoints.Num())
		{
			CurrentPointIndex = 0;
		}
	}

	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	const FVector ForwardDirection = TankBot->GetActorForwardVector();
	const FVector RightDirection = TankBot->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5.f);

	const float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
	const float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection)));

	float RotationValue = 0.f;
	if (ForwardAngle > 5.f)
	{
		RotationValue = 1.f;
	}
	if (RightAngle > 90.f)
	{
		RotationValue = -RotationValue;
	}

	return RotationValue;
}
