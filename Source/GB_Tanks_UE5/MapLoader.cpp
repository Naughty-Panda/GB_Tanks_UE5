// Fill out your copyright notice in the Description page of Project Settings.

#include "MapLoader.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMapLoader::AMapLoader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Mesh"));
	GateMesh->SetupAttachment(RootComponent);

	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Collider"));
	TriggerCollider->SetupAttachment(RootComponent);
	TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerBeginOverlap);

	StatusLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Status Light"));
	StatusLight->SetupAttachment(RootComponent);
}

void AMapLoader::SetActivated(bool InActivated)
{
	bIsActivated = InActivated;
	StatusLight->SetLightColor(InActivated ? ActiveColor : InactiveColor);
}

void AMapLoader::SwitchStatusLight()
{
	StatusLight->SetLightColor(StatusLight->GetLightColor() == InactiveColor ? ActiveColor : InactiveColor);
}

// Called when the game starts or when spawned
void AMapLoader::BeginPlay()
{
	Super::BeginPlay();

	SetActivated(false);
}

void AMapLoader::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActivated)
	{
		return;
	}

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == PlayerPawn)
	{
		UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
	}
}
