// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include <string>

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root component"));

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	//CannonMesh->SetupAttachment(RootComponent);
	RootComponent = CannonMesh;

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Projectile spawn point"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Red, TEXT("Not ready to fire!"));
		return;
	}

	bReadyToFire = false;
	ConsumeAmmo();

	switch (CannonType)
	{
	case ECannonType::FireProjectile: GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Green, TEXT("Fire mode: Projectile"));
		break;
	case ECannonType::FireTrace: GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Blue, TEXT("Fire mode: Trace"));
		break;
	case ECannonType::FireCharge: GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Magenta, TEXT("Fire mode: Charge"));
		break;
	default: break;
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);

	const FString AmmoCounter(FString("Ammo count: ") + std::to_string(AmmoCount).c_str());
	GEngine->AddOnScreenDebugMessage(10, 2.0f, FColor::Yellow, AmmoCounter);
}

void ACannon::FireSpecial()
{
	BurstFire();
}

void ACannon::BurstFire()
{
	// Placeholder
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
}

void ACannon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clearing up timers
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
}

void ACannon::ConsumeAmmo(int32 Count)
{
	AmmoCount = FMath::Clamp(AmmoCount - Count, 0, 10);
}
