// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

void ATankPawn::MoveForward(const float& AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::MoveRight(const float& AxisValue)
{
	TargetRightAxisValue = AxisValue;
}

void ATankPawn::CameraZoom(const float& AxisValue) const
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
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Direction = GetActorForwardVector() * TargetForwardAxisValue + GetActorRightVector() * TargetRightAxisValue;

	if (!Direction.IsZero())
	{
		Direction.Normalize();
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;
		SetActorLocation(TargetLocation, true);
	}
}
