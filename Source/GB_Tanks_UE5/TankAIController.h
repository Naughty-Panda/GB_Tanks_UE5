// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankBot.h"

#include "TankAIController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	//** Player Pawn */
	UPROPERTY()
	APawn* PlayerPawn;
	//** Controlled bot */
	UPROPERTY()
	ATankBot* TankBot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = 1), Category="AI|Move params")
	TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Move params")
	float MovementAccuracy;

	uint8 CurrentPointIndex = INDEX_NONE;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	float CalculateMovementRotation();
};
