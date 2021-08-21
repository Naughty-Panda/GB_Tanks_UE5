// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"

#include "TankPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GB_TANKS_UE5_API ATankPawn : public APawn
{
	GENERATED_BODY()

protected:
	/** Tank body mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TankBody;

	/** Turret body mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretBody;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	/** Input Mapping Context Asset our actor will use */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	/** Input Mapping Priority for the selected Mapping Context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	int32 InputMappingPriority = 1;

	/** Tank move speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 500.0f;

	/** Tank rotation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.0f;

	/** Sets camera angle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FRotator CameraAngle = {270.0f, 0.0f, 0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraZoomStep = 100.0f;

	/** Holds forward and right input axis */
	float TargetForwardAxisValue = 0.0f;
	float TargetRightAxisValue = 0.0f;

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void Rotate(float AxisValue);

	UFUNCTION()
	void CameraZoom(float AxisValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
