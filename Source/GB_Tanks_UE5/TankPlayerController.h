// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Delegates/Delegate.h"

#include "TankPlayerController.generated.h"

class ATankPawn;
class UInputAction;
class AUIManager;
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

	/** Input Action Asset responsible for switching tank's cannon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input Actions")
	UInputAction* TankSwitchCannonInputAction;

	/** Widget class for the Game Over screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DefaultGameOverWidgetClass;

	//** Controlled Pawn */
	UPROPERTY()
	ATankPawn* TankPawn;

	//** Player mouse position */
	UPROPERTY()
	FVector MousePosition;

public:
	/** Delegate for Spawnable Object Widget.
	*	UserWidget can't capture Pointer Event if it occurred outside its geometry!
	*	So we need to broadcast it back to the Widget through, for e.g, Player Controller.
	*/
	FSimpleMulticastDelegate OnLeftMouseButtonUpDelegate;

public:
	ATankPlayerController();
	FVector GetMousePosition() const { return MousePosition; }
	void InitGameOver() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	AUIManager* GetUIManager() const;


protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void MoveTank(const FInputActionValue& Value);
	void CameraZoom(const FInputActionValue& Value);
	void Fire() { TankPawn->Fire(ECannonFireMode::Single); }
	void FireSpecial() { TankPawn->Fire(ECannonFireMode::Burst); }
	void SwitchCannon() { TankPawn->SwitchCannon(); }

	void OnLeftMouseButtonUp() { UE_LOG(LogTemp, Warning, TEXT("LMB Triggered!")); OnLeftMouseButtonUpDelegate.Broadcast(); }
};
