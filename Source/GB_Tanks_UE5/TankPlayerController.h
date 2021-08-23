// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TankPlayerController.generated.h"

class ATankPawn;
class UInputAction;
struct FInputActionValue;
struct FInputActionInstance;

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

	/** Input Action Asset responsible for camera zoom in and out */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input Actions")
	UInputAction* CameraZoomInputAction;

	/** Input Action Asset responsible for tank fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input Actions")
	UInputAction* TankFireInputAction;

	/** Input Action Asset responsible for tank special fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input Actions")
	UInputAction* TankFireSpecialInputAction;

	//** Controlled Pawn */
	UPROPERTY()
	ATankPawn* TankPawn;

	//** Player mouse position */
	UPROPERTY()
	FVector MousePosition;

public:
	ATankPlayerController();
	FVector GetMousePosition() const { return MousePosition; }

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void MoveTank(const FInputActionValue& Value);
	void CameraZoom(const FInputActionValue& Value);
	void Fire();
	void FireSpecial();
};
