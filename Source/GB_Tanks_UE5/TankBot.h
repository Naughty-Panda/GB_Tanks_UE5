// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankBase.h"

#include "TankBot.generated.h"

class ATargetPoint;

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API ATankBot : public ATankBase
{
	GENERATED_BODY()

protected:
	//** Array of patrolling points for NPC to follow */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = 1), Category="AI|Move params")
	TArray<ATargetPoint*> PatrollingPoints;
	//TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Move params")
	float MovementAccuracy = 250.f;

public:
	// Sets default values for this actor's properties
	ATankBot();

	UFUNCTION()
	const TArray<ATargetPoint*>& GetPatrollingPoints() const;

	UFUNCTION()
	float GetMovementAccuracy() const;

	UFUNCTION()
	void SetPatrollingPoints(const TArray<ATargetPoint*>& NewPoints);
};
