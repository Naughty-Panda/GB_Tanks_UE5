// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "GB_Tanks_UE5.h"
#include "TankPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnhancedInputSubsystems.h"

// Sets default values
ATankPawn::ATankPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Tank body and ROOT component
	TankBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = TankBody;

	// Turret body
	TurretBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretBody->SetupAttachment(TankBody);

	//SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(TankBody);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->TargetArmLength = 1200.0f;
	SpringArm->SetRelativeRotation(CameraAngle);

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::MoveRight(float AxisValue)
{
	TargetRightAxisValue = AxisValue;
}

void ATankPawn::Rotate(float AxisValue)
{
	TargetRotationAxisValue = AxisValue;
}

void ATankPawn::CameraZoom(float AxisValue)
{
	// TODO: Switch from Axis to Action Mapping?
	// TODO: Smooth camera zooming using lerp?
	if (AxisValue)
	{
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + CameraZoomStep * AxisValue, 600.0f, 6000.0f);
	}
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ATankPlayerController>(GetController());
}

// PawnClientRestart can run more than once in an Actor's lifetime
void ATankPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Make sure we have a valid Player Controller.
	PlayerController = Cast<ATankPlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: wrong Player Controller Class!"), TEXT(__FUNCTION__));
		return;
	}

	// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s: wrong Enhanced Input Local Player Subsystem!"), TEXT(__FUNCTION__));
		return;
	}

	// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
	Subsystem->ClearAllMappings();

	if (!InputMappingContext)
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s: Input Mapping Context Asset wasn't assigned!"), TEXT(__FUNCTION__));
		return;
	}

	// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
	Subsystem->AddMappingContext(InputMappingContext, InputMappingPriority);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank movement tick
	FVector Direction = GetActorForwardVector() * TargetForwardAxisValue + GetActorRightVector() * TargetRightAxisValue;
	if (!Direction.IsZero())
	{
		Direction.Normalize();
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;
		SetActorLocation(TargetLocation, true);

		// TODO: FInd a better way to reset Enhanced Input's axes.
		TargetForwardAxisValue = TargetRightAxisValue = 0.0f;
	}

	// Tank rotation tick
	CurrentRotationAxisValue = FMath::FInterpConstantTo(CurrentRotationAxisValue, TargetRotationAxisValue, DeltaTime, TankRotInterpSpeed);
	if (CurrentRotationAxisValue)
	{
		float YawRotation = CurrentRotationAxisValue * RotationSpeed * DeltaTime;
		FRotator CurrentRotation = GetActorRotation();
		YawRotation += CurrentRotation.Yaw;
		FRotator NewRotation{0.0f, YawRotation, 0.0f};
		SetActorRotation(NewRotation);

		UE_LOG(LogTanks, Log, TEXT("Current %f Target %f"), CurrentRotationAxisValue, TargetRotationAxisValue);
		TargetRotationAxisValue = 0.0f;
	}

	// Turret rotation tick
	if (PlayerController)
	{
		FVector MousePosition = PlayerController->GetMousePosition();
		float TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePosition).Yaw;
		float CurrentRotation = TurretBody->GetComponentRotation().Yaw;
		//TargetRotation.Roll = CurrentRotation.Roll;
		//TargetRotation.Pitch = CurrentRotation.Pitch;
		TargetRotation = FMath::FInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurrerRotInterpSpeed);
		TurretBody->SetWorldRotation({0.0f, TargetRotation, 0.0f});
	}
}
