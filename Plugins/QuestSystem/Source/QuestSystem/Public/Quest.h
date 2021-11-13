// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "GameFramework/Actor.h"

#include "Quest.generated.h"

class UQuestObjective;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestStatusUpdated, AQuest* UpdatedQuest);

UCLASS()
class QUESTSYSTEM_API AQuest : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Quest")
	FText Name;

	UPROPERTY(EditAnywhere, Category="Quest")
	FText Description;

	UPROPERTY(EditAnywhere, Category="Quest")
	AQuest* PreviousQuest;

public:
	UPROPERTY(EditAnywhere, Category="Quest")
	uint8 bIsStoryQuest :1;
	UPROPERTY(EditAnywhere, Category="Quest")
	uint8 bKeepObjectivesOrder :1;
	UPROPERTY(EditAnywhere, Category="Quest")
	uint8 bIsTaken :1;

	FOnQuestStatusUpdated OnQuestStatusUpdated;

protected:
	UPROPERTY(EditAnywhere, Category="Quest")
	TArray<UQuestObjective*> Objectives;

public:
	// Sets default values for this actor's properties
	AQuest();

protected:
	virtual void BeginPlay() override;

public:
	FText GetQuestName() const { return Name; }
	FText GetQuestDescription() const { return Description; }
	const AQuest* GetPreviousQuest() const { return PreviousQuest; }
	bool IsCompleted() const;

	UFUNCTION(BlueprintPure)
	const TArray<UQuestObjective*>& GetObjectives() const;

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

	UFUNCTION(CallInEditor, Category="Quest")
	void AddInteractionObjective()
	{
		Objectives.Add(NewObject<UInteractionObjective>(this));
	}

	UFUNCTION(CallInEditor, Category="Quest")
	void AddLocationObjective()
	{
		Objectives.Add(NewObject<ULocationObjective>(this));
	}

	UFUNCTION(CallInEditor, Category="Quest")
	void AddCollectionObjective()
	{
		Objectives.Add(NewObject<UCollectionObjective>(this));
	}

#endif
};
