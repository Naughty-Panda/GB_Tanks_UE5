// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectableItem.h"
#include "GB_Tanks_UE5.h"
#include "TankPawn.h"

// Sets default values
ACollectableItem::ACollectableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACollectableItem::OnBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ACollectableItem::OnEndOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();
}

void ACollectableItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                      bool bFromSweep, const FHitResult& HitResult)
{
	UE_LOG(LogTanks, Warning, TEXT("Overlaping..."));

	OverlappedActor = Cast<ATankPawn>(OtherActor) ;
}

void ACollectableItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	UE_LOG(LogTanks, Warning, TEXT("End Overlaping..."));

	OverlappedActor = nullptr;
}
