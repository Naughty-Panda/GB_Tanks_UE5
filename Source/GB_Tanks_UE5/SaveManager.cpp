// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveManager.h"

#include "InventoryComponent.h"
#include "TankBot.h"
#include "TankPawn.h"
#include "TanksSaveGame.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

void USaveManager::Init()
{
	CurrentSave = Cast<UTanksSaveGame>(UGameplayStatics::CreateSaveGameObject(UTanksSaveGame::StaticClass()));

	// Not really working...
	GetWorld()->OnLevelsChanged().AddUObject(this, &USaveManager::LoadInNewLevel);
}

bool USaveManager::IsSaveGameExists(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USaveManager::LoadGame(const FString& SlotName)
{
	if (!IsSaveGameExists(SlotName))
	{
		return;
	}

	FAsyncLoadGameFromSlotDelegate LoadDelegate;
	LoadDelegate.BindUObject(this, &ThisClass::OnGameLoadedAsync);

	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadDelegate);

	// Synchronous load method.
	//CurrentSave = Cast<UTanksSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
}

void USaveManager::SaveGame(const FString& SlotName)
{
	// Save current level name.
	CurrentSave->LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// Save all existing tanks.
	TArray<AActor*> Tanks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATankBase::StaticClass(), Tanks);

	for (AActor* Tank : Tanks)
	{
		RegisterForSave(Tank);
	}

	// Finally saving to slot.
	FAsyncSaveGameToSlotDelegate SaveDelegate;
	SaveDelegate.BindUObject(this, &USaveManager::OnGameSavedAsync);

	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, SlotName, 0, SaveDelegate);

	// Synchronous save method.
	//UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
}

void USaveManager::RegisterForSave(AActor* TankToSave)
{
	if (!TankToSave)
	{
		return;
	}

	if (ATankBase* Tank = Cast<ATankBase>(TankToSave))
	{
		// Collect common data first.
		FTankSaveData SaveData;
		SaveData.Location = Tank->GetActorLocation();
		SaveData.Rotation = Tank->GetActorRotation();
		SaveData.CurrentHealth = Tank->HealthComponent->GetHealth();
		SaveData.CannonType = Tank->GetPrimaryCannon()->GetCannonType();
		SaveData.CannonMaxAmmo = Tank->GetPrimaryCannon()->GetAmmo().MaxAmmo;
		SaveData.CannonCurrentAmmo = Tank->GetPrimaryCannon()->GetAmmo().CurrentAmmo;

		// Check if it's AI Tank.
		if (Tank->IsAIControlled())
		{
			CurrentSave->AISaveData.Add(SaveData);
		}
			// It's a Player Tank.
		else
		{
			// Move all available data into new struct.
			FPlayerTankSaveData PlayerData;
			PlayerData = MoveTemp(SaveData);

			if (Tank->GetSecondaryCannon())
			{
				PlayerData.SecondaryCannonType = Tank->GetSecondaryCannon()->GetCannonType();
				PlayerData.SecondaryCannonMaxAmmo = Tank->GetSecondaryCannon()->GetAmmo().MaxAmmo;
				PlayerData.SecondaryCannonCurrentAmmo = Tank->GetSecondaryCannon()->GetAmmo().CurrentAmmo;
			}

			PlayerData.InventoryItems = CopyTemp(Cast<ATankPawn>(Tank)->GetInventoryComponent()->GetItems());

			CurrentSave->PlayerSaveData = MoveTemp(PlayerData);
		}
	}
}

void USaveManager::LoadInNewLevel()
{
	// Spawn new AI actors.
	for (const auto& TankData : CurrentSave->AISaveData)
	{
		ATankBot* NewTank = GetWorld()->SpawnActor<ATankBot>(TankData.Location, TankData.Rotation);
		// Copy other tank data back...
	}

	// Player Tank is already spawned, we just need to update it.
	const auto& PlayerData = CurrentSave->PlayerSaveData;
	auto* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto* PlayerTank = PC->GetPawn();
	PlayerTank->SetActorLocation(PlayerData.Location, true);
	PlayerTank->SetActorRotation(PlayerData.Rotation);
}

void USaveManager::OnGameLoadedAsync(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentSave = Cast<UTanksSaveGame>(SaveGame);
	OnGameLoaded.Broadcast(SlotName);

	UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentSave->LevelName));
}

void USaveManager::OnGameSavedAsync(const FString& SlotName, const int32 UserIndex, bool bIsSaved)
{
	OnGameSaved.Broadcast(SlotName);
}
