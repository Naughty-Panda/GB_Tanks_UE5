// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "DrawDebugHelpers.h"
#include "GB_Tanks_UE5.h"
#include "Engine/TargetPoint.h"
#include "TankBot.h"

void ATankAIController::TryInit()
{
	TankBot = Cast<ATankBot>(GetPawn());

	if (TankBot)
	{
		MovementAccuracy = TankBot->GetMovementAccuracy();
		PatrollingPoints = TankBot->GetPatrollingPoints();

		CurrentPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;
	}
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	/* TankBot may not be created
	 * at the Controller's BeginPlay
	 * so we have to check this
	 */

	TryInit();

	// const TArray<ATargetPoint*> BotPoints = TankBot->GetPatrollingPoints();
	// const FVector CurrentLocation = TankBot->GetActorLocation();
	//
	// for (const auto Point : BotPoints)
	// {
	// 	const FVector NewPoint = Point->GetActorLocation();
	// 	PatrollingPoints.Add(NewPoint);
	// }
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!TankBot)
	{
		TryInit();

		if (!TankBot)
		{
			return;
		}
	}

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
	if (CurrentPointIndex == INDEX_NONE)
	{
		UE_LOG(LogTanks, Error, TEXT("%s has no Current Waypoint Index! Array size: %d"), *GetName(), PatrollingPoints.Num());
		return 0.f;
	}

	const ATargetPoint* CurrentPointPtr = PatrollingPoints[CurrentPointIndex];
	const FVector CurrentPoint = CurrentPointPtr->GetActorLocation();
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
