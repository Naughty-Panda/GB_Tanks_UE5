// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectilePhysicsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GB_TANKS_UE5_API UProjectilePhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Physics settings")
	float Gravity = -9.8f;

public:
	UProjectilePhysicsComponent();

	UFUNCTION()
	TArray<FVector> GenerateTrajectory(const FVector& StartPos, const FVector& Velocity, float MaxTime, float TimeStep, float MinZVal = 0.f);
};
