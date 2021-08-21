// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "EnhancedInputComponent.h"
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/** Make sure that we are using a UEnhancedInputComponent; if not, the project is not configured correctly. */
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s: wrong Enhanced Input Component Class!"), TEXT(__FUNCTION__));
		return;
	}
	if (!MovementInputAction)
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s: Input Action Asset wasn't assigned correctly!"), TEXT(__FUNCTION__));
		return;
	}

	EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ATankPlayerController::MoveTank);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveTank(const FInputActionValue& Value)
{
	//** Value[1] = FVector Y axis */
	TankPawn->MoveForward(Value[1]);
	//** Value[0] = FVector X axis */
	TankPawn->MoveRight(Value[0]);
	//** Value[2] = FVector Z axis */
	TankPawn->Rotate(Value[2]);
}

void ATankPlayerController::CameraZoom(float AxisValue)
{
	TankPawn->CameraZoom(AxisValue);
}
