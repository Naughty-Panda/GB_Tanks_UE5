// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "GameFramework/Actor.h"

#include "Quest.generated.h"

class UQuestObjective;

UCLASS()
class QUESTSYSTEM_API AQuest : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	TArray<UQuestObjective*> Objectives;

	UPROPERTY(EditAnywhere)
	AQuest* PreviousQuest;

	UPROPERTY(EditAnywhere)
	uint8 bIsStoryQuest :1;
	UPROPERTY(EditAnywhere)
	uint8 bKeepObjectivesOrder :1;
	UPROPERTY(EditAnywhere)
	uint8 bIsTaken :1;

public:
	// Sets default values for this actor's properties
	AQuest();

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void UpdateLocation();

	UFUNCTION(BlueprintCallable)
	void TakeQuest(AActor* Character);

	UFUNCTION()
	void OnObjectiveCompleted(UQuestObjective* Objective);

	/**
	 * Functions to add Objectives.
	 */

#if WITH_EDITOR

	UFUNCTION(CallInEditor)
	void AddInteractionObjective()
	{
		Objectives.Add(NewObject<UInteractionObjective>(this));
	}

	UFUNCTION(CallInEditor)
	void AddLocationObjective()
	{
		Objectives.Add(NewObject<ULocationObjective>(this));
	}

	UFUNCTION(CallInEditor)
	void AddCollectionObjective()
	{
		Objectives.Add(NewObject<UCollectionObjective>(this));
	}

#endif
};
