// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "QuestDescription.generated.h"

class AQuest;
class UTextBlock;
class UScrollBox;
class UObjectiveWidget;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDescription : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestNameText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestDescriptionText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* ObjectivesList;

public:
	void Init(AQuest* Quest);
};
