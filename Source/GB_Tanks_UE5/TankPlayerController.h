// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TankPlayerController.generated.h"

class ATankPawn;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GB_TANKS_UE5_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Enhanced Player Input Component */
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;

	/** Input Action Asset responsible for tank movement and rotation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input Actions")
	UInputAction* MovementInputAction;

	//** Controlled Pawn */
	UPROPERTY()
	ATankPawn* TankPawn;

public:
	ATankPlayerController();

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	void MoveTank(const FInputActionValue& Value);
	void CameraZoom(float AxisValue);
};
