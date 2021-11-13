// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "QuestLogWidget.generated.h"

class UScrollBox;
class UQuestDescription;
class UQuestLogEntry;
class AQuest;
class UQuestListComponent;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestLogWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* StoryQuestList;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* SideQuestList;

	UPROPERTY(meta=(BindWidget))
	UQuestDescription* ActiveQuestDescription;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UQuestLogEntry> QuestLogEntryClass;

	UPROPERTY()
	TMap<AQuest*, UQuestLogEntry*> QuestEntries;

public:
	void Init(UQuestListComponent* QuestList);
	void OnActiveQuestChanged(AQuest* ActiveQuest);
};
