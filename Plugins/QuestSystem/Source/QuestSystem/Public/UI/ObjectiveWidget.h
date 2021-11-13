// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ObjectiveWidget.generated.h"

class UQuestObjective;
class UCheckBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UCheckBox* ObjectiveStatusBox;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ObjectiveDescriptionText;

public:
	void Init(UQuestObjective* Objective);
};
