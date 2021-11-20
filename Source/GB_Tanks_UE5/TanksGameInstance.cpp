// Fill out your copyright notice in the Description page of Project Settings.

#include "TanksGameInstance.h"

#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"

void UTanksGameInstance::Init()
{
	Super::Init();

	GLog->Log(ELogVerbosity::Display, TEXT("UTanksGameInstance::Init seccesfull!"));

	SaveManager = NewObject<USaveManager>(this);
	SaveManager->Init();
}

USaveManager* UTanksGameInstance::GetSaveManager(UObject* WorldContextObject)
{
	UTanksGameInstance* GI = Cast<UTanksGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	return GI ? GI->SaveManager : nullptr;
}
