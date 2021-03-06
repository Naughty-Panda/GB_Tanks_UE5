// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentObjectives.generated.h"

class AQuest;
class UObjectiveWidget;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UCurrentObjectives : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UVerticalBox* ObjectivesList;
	
public:
	UFUNCTION()
	void SetCurrentObjectives(AQuest* Quest);
};
