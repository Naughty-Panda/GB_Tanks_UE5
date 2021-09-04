// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "DamageTaker.h"
#include "GB_Tanks_UE5.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	CannonMesh->SetupAttachment(RootComponent);
	CannonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Projectile spawn point"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::Fire(ECannonFireMode FireMode)
{
	if (!IsReadyToFire())
	{
		UE_LOG(LogTanks, Error, TEXT("Not ready to fire!"));
		return;
	}

	bReadyToFire = false;
	--Ammo;

	UE_LOG(LogTanks, Log, TEXT("Ammo: %i/%i"), Ammo.CurrentAmmo, Ammo.MaxAmmo);

	switch (FireMode)
	{
	case ECannonFireMode::Single: FireSingle();
		break;
	case ECannonFireMode::Burst: FireBurst();
		break;
	default: break;
	}
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

void ACannon::FireSingle()
{
	switch (CannonType)
	{
	case ECannonType::ProjectileCannon:
		UE_LOG(LogTanks, Warning, TEXT("Projectile"));
		ShootProjectile();
		break;
	case ECannonType::TraceCannon:
		UE_LOG(LogTanks, Warning, TEXT("Trace"));
		ShootTrace();
		break;
	default: break;
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRateSingle, false);
}

void ACannon::FireBurst()
{
	if (BurstFireShotsLeft-- != 0)
	{
		switch (CannonType)
		{
		case ECannonType::ProjectileCannon:
			UE_LOG(LogTanks, Warning, TEXT("Projectile"));
			ShootProjectile();
			break;
		case ECannonType::TraceCannon:
			UE_LOG(LogTanks, Warning, TEXT("Trace"));
			ShootTrace();
			break;
		default: break;
		}

		GetWorld()->GetTimerManager().SetTimer(BurstFireTimerHandle, this, &ACannon::FireBurst, 1 / FireRateBurst / BurstFireShots, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRateBurst, false);
	}
}

void ACannon::Reload()
{
	bReadyToFire = true;
	BurstFireShotsLeft = BurstFireShots;
}

void ACannon::ShootProjectile() const
{
	if (AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(DefaultProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
	                                                                  ProjectileSpawnPoint->GetComponentRotation()))
	{
		Projectile->SetInstigator(GetInstigator());
		Projectile->Start();
	}
}

void ACannon::ShootTrace()
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	UE_LOG(LogTanks, Log, TEXT("TraceComplex: %i, ReturnPhysicalMaterial: %i"), TraceParams.bTraceComplex, TraceParams.bReturnPhysicalMaterial);
	FVector Start = ProjectileSpawnPoint->GetComponentLocation();
	FVector End = ProjectileSpawnPoint->GetForwardVector() * FireRange + Start;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Green, false, 2.5f, 0, 5.0f);
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(HitActor))
			{
				AActor* MyInstigator = GetInstigator();
				if (MyInstigator != HitActor)
				{
					FDamageData DamageData;
					DamageData.Instigator = MyInstigator;
					DamageData.DamageDealer = this;
					DamageData.DamageValue = FireDamage;

					DamageTaker->TakeDamage(DamageData);
				}
			}
			else
			{
				HitActor->Destroy();
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.5f, 0, 5.0f);
	}
}
