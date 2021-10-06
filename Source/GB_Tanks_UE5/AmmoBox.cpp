// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoBox.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAmmoBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, HitResult);

	if (OverlappedActor->IsPlayerControlled())
	{
		if (ACannon* Cannon = OverlappedActor->GetPrimaryCannon())
		{
			if (!Cannon->HasFullAmmo())
			{
				Cannon->AddAmmo(AmmoCount);
				Destroy();
			}
		}
	}
}
