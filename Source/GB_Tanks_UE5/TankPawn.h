// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "Cannon.h"

#include "TankPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ATankPlayerController;
class UArrowComponent;

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonAttachPoint;

	//** Default class for tank's cannon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ACannon> DefaultCannonClass;

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

	/** Tank rotation interpolation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float TankRotInterpSpeed = 1.5f;

	/** Turret rotation interpolation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float TurrerRotInterpSpeed = 150.0f;

	/** Sets camera angle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FRotator CameraAngle = {270.0f, 0.0f, 0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraZoomStep = 100.0f;

private:
	UPROPERTY()
	ATankPlayerController* PlayerController;

	//** Tank Cannons */
	UPROPERTY()
	ACannon* PrimaryCannon;
	UPROPERTY()
	ACannon* SecondaryCannon;

	/** Holds forward, right and rotation axes provided by user input */
	float TargetForwardAxisValue = 0.0f;
	float TargetRightAxisValue = 0.0f;
	float TargetRotationAxisValue = 0.0f;
	float CurrentRotationAxisValue = 0.0f;

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Rotate(float AxisValue);
	void CameraZoom(float AxisValue);
	void Fire(ECannonFireMode FireMode);
	void SwitchCannon();
	void SetupCannon(TSubclassOf<ACannon> InCannon);
	ACannon& GetPrimaryCannon() const;
	ACannon& GetSecondaryCannon() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
