// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveManager.h"

#include "TanksSaveGame.h"
#include "Kismet/GameplayStatics.h"

void USaveManager::Init()
{
	CurrentSave = Cast<UTanksSaveGame>(UGameplayStatics::CreateSaveGameObject(UTanksSaveGame::StaticClass()));
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
	FAsyncSaveGameToSlotDelegate SaveDelegate;
	SaveDelegate.BindUObject(this, &USaveManager::OnGameSavedAsync);

	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, SlotName, 0, SaveDelegate);

	// Synchronous save method.
	//UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
}

void USaveManager::OnGameLoadedAsync(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentSave = Cast<UTanksSaveGame>(SaveGame);
	OnGameLoaded.Broadcast(SlotName);
}

void USaveManager::OnGameSavedAsync(const FString& SlotName, const int32 UserIndex, bool bIsSaved)
{
	OnGameSaved.Broadcast(SlotName);
}
