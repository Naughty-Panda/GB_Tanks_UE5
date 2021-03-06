// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SaveManager.generated.h"

class UTanksSaveGame;
class USaveGame;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveAction, const FString&, SlotName);

UCLASS()
class GB_TANKS_UE5_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UTanksSaveGame* CurrentSave;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> SaveSlots;

	UPROPERTY(BlueprintAssignable)
	FOnSaveAction OnGameSaved;

	UPROPERTY(BlueprintAssignable)
	FOnSaveAction OnGameLoaded;

public:
	void Init();

	UFUNCTION(BlueprintPure)
	static bool IsSaveGameExists(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void LoadGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void SaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void SaveJsonFile();

	UFUNCTION(BlueprintCallable)
	void ReadJsonFile();

	UFUNCTION(BlueprintCallable)
	void SaveXMLFile();

	UFUNCTION(BlueprintCallable)
	void ReadXMLFile();

	UFUNCTION(BlueprintCallable)
	void RegisterForSave(AActor* TankToSave);

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetSaveSlots() const;

	UFUNCTION()
	void LoadInNewLevel();

protected:
	void OnGameLoadedAsync(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);
	void OnGameSavedAsync(const FString& SlotName, const int32 UserIndex, bool bIsSaved);

	void WriteSaveSlotsToDisk() const;
	void WriteSaveSlotsToDisk2() const;

private:
	void FPathsExamples();
};
