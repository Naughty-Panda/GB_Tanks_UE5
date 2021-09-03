// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBase.h"
#include "GB_Tanks_UE5.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATankBase::ATankBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	RootComponent = BodyMesh;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);


	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonAttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon attachment point"));
	CannonAttachPoint->SetupAttachment(TurretMesh);
}

void ATankBase::MoveForward(const float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankBase::MoveRight(const float AxisValue)
{
	TargetRightAxisValue = AxisValue;
}

void ATankBase::Rotate(const float AxisValue)
{
	TargetRotationAxisValue = AxisValue;
}

void ATankBase::Fire(const ECannonFireMode FireMode)
{
	if (PrimaryCannon)
	{
		PrimaryCannon->Fire(FireMode);
	}
}

void ATankBase::SwitchCannon()
{
	Swap(PrimaryCannon, SecondaryCannon);
}

void ATankBase::SetupCannon(const TSubclassOf<ACannon> InCannon)
{
	if (!InCannon)
	{
		return;
	}

	if (PrimaryCannon)
	{
		PrimaryCannon->Destroy();
		PrimaryCannon = nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = SpawnParameters.Instigator = this;

	PrimaryCannon = GetWorld()->SpawnActor<ACannon>(InCannon, SpawnParameters);
	PrimaryCannon->AttachToComponent(CannonAttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

ACannon* ATankBase::GetPrimaryCannon() const
{
	return PrimaryCannon;
}

ACannon* ATankBase::GetSecondaryCannon() const
{
	return SecondaryCannon;
}

// Called when the game starts or when spawned
void ATankBase::BeginPlay()
{
	Super::BeginPlay();

	ensure(DefaultCannonClass);
	SetupCannon(DefaultCannonClass);
}

// Called every frame
void ATankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPawnControlled())
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();

	// Turret rotation tick
	/*if (PlayerController)
	{
		FVector MousePosition = PlayerController->GetMousePosition();
		float TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, MousePosition).Yaw;
		float CurrentRotation = TurretMesh->GetComponentRotation().Yaw;
		//TargetRotation.Roll = CurrentRotation.Roll;
		//TargetRotation.Pitch = CurrentRotation.Pitch;
		TargetRotation = FMath::FInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurrerRotInterpSpeed);
		TurretMesh->SetWorldRotation({0.0f, TargetRotation, 0.0f});
	}*/

	// Movement tick
	FVector Direction = GetActorForwardVector() * TargetForwardAxisValue + GetActorRightVector() * TargetRightAxisValue;
	if (!Direction.IsZero())
	{
		Direction.Normalize();
		const FVector TargetLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;
		SetActorLocation(TargetLocation, true);

		// TODO: FInd a better way to reset Enhanced Input's axes.
		TargetForwardAxisValue = TargetRightAxisValue = 0.0f;
	}

	// Base rotation tick
	CurrentRotationAxisValue = FMath::FInterpConstantTo(CurrentRotationAxisValue, TargetRotationAxisValue, DeltaTime, BaseRotInterpSpeed);
	if (CurrentRotationAxisValue)
	{
		float YawRotation = CurrentRotationAxisValue * RotationSpeed * DeltaTime;
		const FRotator CurrentRotation = GetActorRotation();
		YawRotation += CurrentRotation.Yaw;
		const FRotator NewRotation{0.0f, YawRotation, 0.0f};
		SetActorRotation(NewRotation);

		UE_LOG(LogTanks, Log, TEXT("Current %f Target %f"), CurrentRotationAxisValue, TargetRotationAxisValue);
		TargetRotationAxisValue = 0.0f;
	}
}
