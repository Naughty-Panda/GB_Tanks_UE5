// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

class UProjectilePhysicsComponent;

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UProjectilePhysicsComponent* PhysicsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UParticleSystemComponent* TrailEffect;

	UPROPERTY(BlueprintReadWrite, Category="Movement params")
	FVector MoveVector;

	UPROPERTY(BlueprintReadWrite, Category="Movement params")
	TArray<FVector> CurrentTrajectory;

	UPROPERTY(BlueprintReadWrite, Category="Movement params")
	int32 TrajectoryPointIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	float MovementAccuracy = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Trajectory")
	float TrajectorySimMaxTime = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Trajectory")
	float TrajectorySimTimeStep = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Trajectory")
	float TrajectorySimSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Trajectory")
	bool bShowTrajectory = true;

public:
	APhysicsProjectile();
	virtual void Start() override;

protected:
	virtual void Move() override;
};
