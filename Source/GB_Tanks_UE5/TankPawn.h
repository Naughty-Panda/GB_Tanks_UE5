// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankBase.h"
#include "InputMappingContext.h"

#include "TankPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ATankPlayerController;
class UCameraShakeBase;
class UForceFeedbackEffect;

UCLASS()
class GB_TANKS_UE5_API ATankPawn : public ATankBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit Feedback")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit Feedback")
	UForceFeedbackEffect* HitForceFeedback;

	/** Input Mapping Context Asset our actor will use */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	/** Input Mapping Priority for the selected Mapping Context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	int32 InputMappingPriority = 1;

	/** Sets camera angle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FRotator CameraAngle = {270.0f, 0.0f, 0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraMinDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraMaxDistance = 6000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraZoomStep = 100.0f;

private:
	UPROPERTY()
	ATankPlayerController* PlayerController;

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	void CameraZoom(float AxisValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;
	virtual void Destroyed() override;

	virtual void DamageTaken(float DamageValue) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
