// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "DamageTaker.h"
#include "GB_Tanks_UE5.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshBeginOverlap);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	SetLifeSpan(FlyRange / MoveSpeed);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AProjectile::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                     bool bFromSweep, const FHitResult& HitResult)
{
	UE_LOG(LogTanks, Warning, TEXT("%s begin overlap with %s"), *GetName(), *OtherActor->GetName());
	if (OtherComponent && OtherComponent->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(OtherActor))
	{
		AActor* MyInstigator = GetInstigator();
		if (MyInstigator != OtherActor)
		{
			FDamageData DamageData;
			DamageData.Instigator = MyInstigator;
			DamageData.DamageValue = Damage;
			DamageData.DamageDealer = this;

			DamageTaker->TakeDamage(DamageData);
		}
	}
	else if (OtherComponent && OtherComponent->IsSimulatingPhysics())
	{
		const FVector ForceVector = GetActorForwardVector();
		OtherComponent->AddImpulseAtLocation(ForceVector * PushImpulse, HitResult.ImpactPoint);
	}

	Destroy();
}

void AProjectile::Move()
{
	FVector NextLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(NextLocation);
}
