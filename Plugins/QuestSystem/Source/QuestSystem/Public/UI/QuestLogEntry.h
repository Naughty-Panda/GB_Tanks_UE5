// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "QuestLogEntry.generated.h"

class UTextBlock;
class UButton;
class UBorder;

/**
 * Entry for the Quest Log Widget
 */
UCLASS()
class QUESTSYSTEM_API UQuestLogEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	FSimpleDelegate OnQuestSelected;

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestText;

	UPROPERTY(meta=(BindWidget))
	UButton* QuestSelectButton;

	UPROPERTY(meta=(BindWidget))
	UBorder* SelectionBorder;
	
public:
	virtual void NativeConstruct() override;

public:
	void SetQuestText(const FText& QuestText);
	void SetIsSelected(bool bIsSelected);

protected:
	UFUNCTION()
	void SelectQuest();
};
