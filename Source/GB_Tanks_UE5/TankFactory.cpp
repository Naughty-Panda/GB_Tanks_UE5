// Fill out your copyright notice in the Description page of Project Settings.

#include "TankFactory.h"
#include "Kismet/GameplayStatics.h"
#include "GB_Tanks_UE5.h"
#include "TankBot.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "MapLoader.h"

// Sets default values
ATankFactory::ATankFactory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(RootComponent);
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DestroyedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destroyed Mesh"));
	DestroyedMesh->SetupAttachment(RootComponent);
	DestroyedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestroyedMesh->SetVisibility(false);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);

	TankSpawningPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Tank Spawn Point"));
	TankSpawningPoint->SetupAttachment(BuildingMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankFactory::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &ATankFactory::DamageTaken);
}

void ATankFactory::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::SpawnNewTank()
{
	FTransform SpawnTransform(TankSpawningPoint->GetComponentRotation(), TankSpawningPoint->GetComponentLocation());
	ATankBot* NewTank = GetWorld()->SpawnActorDeferred<ATankBot>(DefaultTankSpawnClass, SpawnTransform, this, nullptr,
	                                                             ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	NewTank->SetPatrollingPoints(TankWaypoint);

	UGameplayStatics::FinishSpawningActor(NewTank, SpawnTransform);
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	SpawnDelegate.BindLambda([&]()
	{
		SpawnNewTank();
	});

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnDelegate, SpawnRate, true);

	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetActivated(false);
	}
}

void ATankFactory::Destroyed()
{
	Super::Destroyed();
}

void ATankFactory::Die()
{
	// Destroy();

	BuildingMesh->ToggleVisibility();
	DestroyedMesh->ToggleVisibility();

	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetActivated(true);
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionParticleEffect, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());

	SpawnDelegate.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void ATankFactory::DamageTaken(float DamageValue)
{
	UE_LOG(LogTanks, Log, TEXT("Factory %s takes %d damage. Health: %d"), *GetName(), DamageValue, HealthComponent->GetHealth());
}
