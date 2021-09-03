// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"

#include "TankBase.generated.h"

class UArrowComponent;
class UBoxComponent;

UCLASS()
class GB_TANKS_UE5_API ATankBase : public APawn
{
	GENERATED_BODY()

public:
	//** Main collider for the turret */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Collider;

	/** Body mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	/** Turret body mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonAttachPoint;

	//** Default class for tank's cannon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ACannon> DefaultCannonClass;

	/** Tank move speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 500.0f;

	/** Tank rotation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.0f;

	/** Tank rotation interpolation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float BaseRotInterpSpeed = 1.5f;

	/** Turret rotation interpolation speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float TurrerRotInterpSpeed = 150.0f;

private:
	//** Cannons */
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
	ATankBase();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Rotate(float AxisValue);
	void Fire(ECannonFireMode FireMode);
	void SwitchCannon();
	void SetupCannon(TSubclassOf<ACannon> InCannon);
	ACannon* GetPrimaryCannon() const;
	ACannon* GetSecondaryCannon() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
