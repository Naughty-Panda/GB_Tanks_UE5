// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem.h"

void FInventoryItemInfo::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	Super::OnDataTableChanged(InDataTable, InRowName);
	ID = InRowName;
	UE_LOG(LogTemp, Error, TEXT(__FUNCTION__));
}

void FInventoryItemInfo::OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems)
{
	Super::OnPostDataImport(InDataTable, InRowName, OutCollectedImportProblems);
	ID = InRowName;
	UE_LOG(LogTemp, Error, TEXT(__FUNCTION__));
}
