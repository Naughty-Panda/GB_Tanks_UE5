// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBox.h"

#include "GB_Tanks_UE5.h"
#include "TankPawn.h"

// Sets default values
AWeaponBox::AWeaponBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AWeaponBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                bool bFromSweep, const FHitResult& HitResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, HitResult);

	//Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	//OtherActor->GetNetOwningPlayer()->GetPlayerController(GetWorld())->GetPawn())
	if (OverlappedActor->IsPlayerControlled())
	{
		if (!Cannon)
		{
			UE_LOG(LogTanks, Error, TEXT("Cannon asset wasn't assigned!"));
		}

		OverlappedActor->SetupCannon(Cannon);
		Destroy();
	}
}
