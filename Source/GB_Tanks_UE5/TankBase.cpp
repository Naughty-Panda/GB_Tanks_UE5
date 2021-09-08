// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBase.h"
#include "GB_Tanks_UE5.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATankBase::ATankBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	RootComponent = BodyMesh;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);

	VisibilityRange = CreateDefaultSubobject<USphereComponent>(TEXT("Field of view"));
	VisibilityRange->SetupAttachment(RootComponent);
	VisibilityRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisibilityRange->SetCollisionObjectType(ECC_WorldDynamic);
	VisibilityRange->SetCollisionResponseToAllChannels(ECR_Ignore);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonAttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon attachment point"));
	CannonAttachPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankBase::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &ATankBase::DamageTaken);
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

/**
* Compares Direction to Target with current direction of a cannon
* @param TargetLocation Location of a target
*/
bool ATankBase::IsAimedAtLocation(const FVector& TargetLocation) const
{
	const FVector TargetingDirection = TurretMesh->GetForwardVector();
	FVector DirectionToTarget = TargetLocation - GetActorLocation();
	DirectionToTarget.Normalize();
	const float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToTarget)));
	return AimAngle < TargetingAccuracy;
}

bool ATankBase::HasLineOfSightOnTarget(const APawn* Target) const
{
	//const APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	const FVector TargetPosition = Cast<ATankBase>(Target)->TurretMesh->GetComponentLocation();
	const FVector PawnPosition = TurretMesh->GetComponentLocation();

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("Visibility Trace")), false, this);

	GetWorld()->LineTraceSingleByChannel(HitResult, PawnPosition, TargetPosition, ECC_Visibility, TraceParams);

	if (HitResult.GetActor())
	{
		DrawDebugLine(GetWorld(), PawnPosition, HitResult.Location, FColor::Cyan, false, 0.5f, 1, 15.0f);
		return HitResult.GetActor() == Target;
	}

	DrawDebugLine(GetWorld(), PawnPosition, TargetPosition, FColor::Orange, false, 0.5f, 1, 15.0f);
	return false;
}

/*bool ATankBase::IsTargetInRange(const APawn* Target) const
{
	return FVector::Distance(Target->GetActorLocation(), GetActorLocation()) < TargetingRange;
}*/

/**
* Locks on target if it is in range and shoots if a cannon is aimed
* @param Target APawn* to a target
*/
void ATankBase::LockOnTarget(const APawn* Target)
{
	//if (IsTargetInRange(Target))
	if (bIsTargetInRange)
	{
		if (HasLineOfSightOnTarget(Target))
		{
			const FVector TargetLocation = Target->GetActorLocation();
			RotateTurretTo(TargetLocation);

			if (IsAimedAtLocation(TargetLocation) && PrimaryCannon && PrimaryCannon->IsReadyToFire())
			{
				PrimaryCannon->Fire(ECannonFireMode::Single);
			}
		}
	}
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

void ATankBase::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

FVector ATankBase::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

void ATankBase::RotateTurretTo(const FVector& TargetPosition)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TurrerRotInterpSpeed));
}

// Called when the game starts or when spawned
void ATankBase::BeginPlay()
{
	Super::BeginPlay();

	ensure(DefaultCannonClass);
	SetupCannon(DefaultCannonClass);

	// Setting up visibility range Sphere Component for AI controlled pawns
	if (bIsAIControlled)
	{
		VisibilityRange->SetSphereRadius(TargetingRange);
		VisibilityRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		VisibilityRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		VisibilityRange->OnComponentBeginOverlap.AddDynamic(this, &ATankBase::OnEnteringVisibilityRange);
		VisibilityRange->OnComponentEndOverlap.AddDynamic(this, &ATankBase::OnLeavingVisibilityRange);
	}
}

void ATankBase::Destroyed()
{
	Super::Destroyed();

	if (PrimaryCannon)
	{
		PrimaryCannon->Destroy();
	}

	if (SecondaryCannon)
	{
		SecondaryCannon->Destroy();
	}
}

void ATankBase::Die()
{
	Destroy();
}

void ATankBase::DamageTaken(float DamageValue)
{
	UE_LOG(LogTanks, Warning, TEXT("%s taking damage: %f"), *GetName(), DamageValue);
}

void ATankBase::OnEnteringVisibilityRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                          bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UE_LOG(LogTanks, Warning, TEXT("Player begin overlap..."));
		bIsTargetInRange = true;
	}
}

void ATankBase::OnLeavingVisibilityRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UE_LOG(LogTanks, Warning, TEXT("Player end overlap..."));
		bIsTargetInRange = false;
	}
}

// Called every frame
void ATankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPawnControlled())
	{
		return;
	}

	const FVector CurrentLocation = GetActorLocation();

	// Turret rotation tick is handled by its children!
	// Stationary Turret has a timer for targeting,
	// TankBot allows TankAIController to track targets.

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
