// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "EnhancedInputComponent.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIManager.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;

	// Allows to capture Key presses events.
	bEnableClickEvents = true;
}

void ATankPlayerController::InitGameOver() const
{
	if (AUIManager* UIManager = GetUIManager())
	{
		UIManager->SetActiveWidget(EWidgetType::WT_GameOverMenu);
	}
}

AUIManager* ATankPlayerController::GetUIManager() const
{
	return Cast<AUIManager>(GetHUD());
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
	if (!MovementInputAction || !CameraZoomInputAction || !TankFireInputAction || !TankFireSpecialInputAction || !TankSwitchCannonInputAction)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Input Action Assets wasn't assigned correctly! Check BP_TankPlayerController/Input Actions."), TEXT(__FUNCTION__));
		return;
	}

	EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ATankPlayerController::MoveTank);
	EnhancedInputComponent->BindAction(CameraZoomInputAction, ETriggerEvent::Started, this, &ATankPlayerController::CameraZoom);
	EnhancedInputComponent->BindAction(TankFireInputAction, ETriggerEvent::Started, this, &ATankPlayerController::Fire);
	EnhancedInputComponent->BindAction(TankFireSpecialInputAction, ETriggerEvent::Started, this, &ATankPlayerController::FireSpecial);
	EnhancedInputComponent->BindAction(TankSwitchCannonInputAction, ETriggerEvent::Started, this, &ATankPlayerController::SwitchCannon);
	//EnhancedInputComponent->BindAction(TankFireInputAction, ETriggerEvent::Completed, this, &ATankPlayerController::OnLeftMouseButtonUp);

	// Binding for the Spawnable Object Widget.
	//EnhancedInputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Released, this, &ATankPlayerController::OnLeftMouseButtonUp);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MouseDirection;
	const FVector PawnPosition = TankPawn->GetActorLocation();
	DeprojectMousePositionToWorld(MousePosition, MouseDirection);
	MousePosition.Z = PawnPosition.Z;
	FVector Direction = MousePosition - PawnPosition;
	Direction.Normalize();
	MousePosition = PawnPosition + Direction * 1000.0f;

	DrawDebugLine(GetWorld(), PawnPosition, MousePosition, FColor::Green, false, 0.1f, 0.0f, 5.0f);

	// TODO: Should be moved into UIManager.
	// Update player position on minimap.
	if (AUIManager* UIManager = GetUIManager())
	{
		const FVector2D PlayerPosition(PawnPosition.X, PawnPosition.Y);
		UIManager->UpdatePlayerPositionOnMinimap(PlayerPosition);
	}
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

void ATankPlayerController::CameraZoom(const FInputActionValue& Value)
{
	TankPawn->CameraZoom(Value[0]);
}
