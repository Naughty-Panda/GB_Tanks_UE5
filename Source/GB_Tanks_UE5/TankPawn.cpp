// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include "TankPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"

ACannon& ATankPawn::GetPrimaryCannon() const
{
	return *PrimaryCannon;
}

ACannon& ATankPawn::GetSecondaryCannon() const
{
	return *SecondaryCannon;
}

// Sets default values
ATankPawn::ATankPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsAIControlled = false;

	//SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
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

void ATankPawn::CameraZoom(float AxisValue)
{
	// TODO: Smooth camera zooming using lerp?
	if (AxisValue)
	{
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + CameraZoomStep * AxisValue, CameraMinDistance, CameraMaxDistance);
	}
}

void ATankPawn::SwitchCannon()
{
	Swap(PrimaryCannon, SecondaryCannon);
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> InCannon)
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

void ATankPawn::DamageTaken(float DamageValue)
{
	Super::DamageTaken(DamageValue);

	if (GetWorld()->GetFirstPlayerController()->GetPawn() == this)
	{
		if (HitForceFeedback)
		{
			FForceFeedbackParameters FeedbackParams;
			FeedbackParams.bLooping = false;
			FeedbackParams.Tag = TEXT("HitForceFeedbackParams");
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(HitForceFeedback, FeedbackParams);
		}

		if (HitCameraShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
		}
	}
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Turret rotation tick
	if (PlayerController)
	{
		const FVector MousePosition = PlayerController->GetMousePosition();
		RotateTurretTo(MousePosition);
	}
}
