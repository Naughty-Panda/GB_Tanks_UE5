// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveManager.h"

#include "InventoryComponent.h"
#include "TankBot.h"
#include "TankPawn.h"
#include "TanksSaveGame.h"
#include "Components/HealthComponent.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "XmlParser/Public/FastXml.h"
#include "XmlParser/Public/XmlFile.h"

void USaveManager::Init()
{
	CurrentSave = Cast<UTanksSaveGame>(UGameplayStatics::CreateSaveGameObject(UTanksSaveGame::StaticClass()));

	// Not really working...
	GetWorld()->OnLevelsChanged().AddUObject(this, &USaveManager::LoadInNewLevel);

	const FString SlotsPaths = FPaths::ProjectSavedDir() + TEXT("SaveGames/Slots.txt");

	IFileManager& FileManager = IFileManager::Get();
	if (FileManager.FileExists(*SlotsPaths))
	{
		if (FFileHelper::LoadFileToStringArray(SaveSlots, *SlotsPaths))
		{
			for (const auto& Slot : SaveSlots)
			{
				GLog->Log(ELogVerbosity::Warning, TEXT("Slot found: ") + Slot);
			}
		}
	}
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

void USaveManager::SaveJsonFile()
{
	FTankSaveData SaveData;
	SaveData.Location = FVector::UpVector;
	SaveData.Rotation = FRotator::ZeroRotator;

	FString JsonString;

	if (FJsonObjectConverter::UStructToJsonObjectString(SaveData, JsonString))
	{
		const FString JsonFile = FPaths::ProjectSavedDir() + TEXT("SaveGames/JsonFile.json");
		FFileHelper::SaveStringToFile(JsonString, *JsonFile);
	}
}

void USaveManager::ReadJsonFile()
{
	const FString JsonFile = FPaths::ProjectSavedDir() + TEXT("SaveGames/JsonFile.json");

	IFileManager& FileManager = IFileManager::Get();
	if (!FileManager.FileExists(*JsonFile))
	{
		return;
	}

	FString JsonString;

	if (FFileHelper::LoadFileToString(JsonString, *JsonFile))
	{
		FTankSaveData SaveData;

		if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &SaveData))
		{
			GLog->Log(ELogVerbosity::Warning, TEXT("Json file loaded!"));
		}
	}
}

void USaveManager::SaveXMLFile()
{
	// For testing only.
	const FString XmlFilePath = FPaths::ProjectSavedDir() + TEXT("SaveGames/JsonFile.json");

	FXmlFile XmlFile;
	FXmlAttribute Attribute("Name", "Value");
	XmlFile.Save(XmlFilePath);
}

void USaveManager::ReadXMLFile()
{
	// For testing only.
	const FString XmlFilePath = FPaths::ProjectSavedDir() + TEXT("SaveGames/JsonFile.json");

	FXmlFile XmlFile;
	XmlFile.LoadFile(XmlFilePath);
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

TArray<FString> USaveManager::GetSaveSlots() const
{
	TArray<FString> TempSlots;
	TArray<FString> TempSlots2;

	IFileManager& FileManager = IFileManager::Get();
	IPlatformFile& FileManager2 = FPlatformFileManager::Get().GetPlatformFile();
	FileManager.FindFiles(TempSlots, *FPaths::ProjectSavedDir(), TEXT(".sav"));
	FileManager2.FindFiles(TempSlots2, *FPaths::ProjectSavedDir(), TEXT(".sav"));

	for (const auto& Slot : TempSlots)
	{
		GLog->Log(ELogVerbosity::Warning, TEXT("Slot found: ") + Slot);
	}

	for (const auto& Slot : TempSlots2)
	{
		GLog->Log(ELogVerbosity::Warning, TEXT("Slot found: ") + Slot);
	}

	return SaveSlots;
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
	if (!SaveSlots.Contains(SlotName))
	{
		SaveSlots.Add(SlotName);
		WriteSaveSlotsToDisk();
	}

	OnGameSaved.Broadcast(SlotName);
}

void USaveManager::WriteSaveSlotsToDisk() const
{
	const FString SlotsPaths = FPaths::ProjectSavedDir() + TEXT("SaveGames/Slots.txt");

	if (FFileHelper::SaveStringArrayToFile(SaveSlots, *SlotsPaths))
	{
		for (const auto& Slot : SaveSlots)
		{
			GLog->Log(ELogVerbosity::Warning, TEXT("Slot saved: ") + Slot);
		}
	}
}

void USaveManager::WriteSaveSlotsToDisk2() const
{
	const FString SlotsPaths = FPaths::ProjectSavedDir() + TEXT("SaveGames/Slots.txt");

	FString ExistingSaveSlotArray = "";
	for (const FString& Slot : SaveSlots)
	{
		ExistingSaveSlotArray += Slot + ",";
	}

	FFileHelper::SaveStringToFile(ExistingSaveSlotArray, *SlotsPaths, FFileHelper::EEncodingOptions::ForceUnicode,
	                              &IFileManager::Get(), FILEWRITE_EvenIfReadOnly);
}

void USaveManager::FPathsExamples()
{
	FString ProjectDir = FPaths::ProjectDir();
	FString TestFile = TEXT("G:/Unreal Projects/GB_Tanks_UE5/Saved/Logs/TestFile.txt");

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	// File Handle should be deleted manually!
	IFileHandle* FileHandle = FileManager.OpenRead(*TestFile);

	if (FileHandle)
	{
		int32 TestInt = INDEX_NONE;
		uint8* ByteBuffer = reinterpret_cast<uint8*>(&TestInt);

		FileHandle->Read(ByteBuffer, sizeof(int32));

		delete FileHandle;
		FileHandle = nullptr;
	}

	// Here is some functionality as well.
	//FFileHelper FileHelper;
}
