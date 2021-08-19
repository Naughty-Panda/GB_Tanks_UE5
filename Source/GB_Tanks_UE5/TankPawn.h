// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GB_TANKS_UE5_API ATankPawn : public APawn
{
	GENERATED_BODY()

protected:
	// Tank body mesh component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TankBody;

	// Turret body mesh component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretBody;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	// Tank move speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 500.0f;

	// Tank rotation speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.0f;

	// Sets camera angle
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FRotator CameraAngle = {270.0f, 0.0f, 0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraZoomStep = 100.0f;

	// Holds forward and right input axis
	float TargetForwardAxisValue = 0.0f;
	float TargetRightAxisValue = 0.0f;

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION()
	void MoveForward(const float& AxisValue);

	UFUNCTION()
	void MoveRight(const float& AxisValue);

	UFUNCTION()
	void CameraZoom(const float& AxisValue) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
