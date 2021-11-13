// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"
#include "QuestObjective.h"

// Sets default values
AQuest::AQuest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bIsTaken = false;
	bIsStoryQuest = false;
	bKeepObjectivesOrder = false;
}

void AQuest::BeginPlay()
{
	Super::BeginPlay();
}

bool AQuest::IsCompleted() const
{
	// Check only last objective if objectives order is required.
	if (bKeepObjectivesOrder)
	{
		return Objectives.Last()->bIsCompleted;
	}

	for (const auto* Objective : Objectives)
	{
		if (!Objective->bIsCompleted)
		{
			return false;
		}
	}

	return true;
}

const TArray<UQuestObjective*>& AQuest::GetObjectives() const
{
	return Objectives;
}

void AQuest::UpdateLocation()
{
	if (AActor* ParentActor = GetParentActor())
	{
		SetActorLocation(ParentActor->GetActorLocation());
	}
}

void AQuest::TakeQuest(AActor* Character)
{
	if (bIsTaken)
	{
		return;
	}

	for (int32 i = 0; i < Objectives.Num(); ++i)
	{
		auto* Objective = Objectives[i];

		if (!Objective)
		{
			continue;
		}

		Objective->ActivateObjective(Character);
		Objective->bCanBeCompleted = i == 0 || !bKeepObjectivesOrder;
		Objective->OnObjectiveCompleted.AddDynamic(this, &ThisClass::OnObjectiveCompleted);
	}

	bIsTaken = true;
}

void AQuest::OnObjectiveCompleted(UQuestObjective* Objective)
{
	if (bKeepObjectivesOrder)
	{
		int32 Index;
		if (Objectives.Find(Objective, Index) && Objectives.IsValidIndex(Index + 1))
		{
			Objectives[Index + 1]->bCanBeCompleted = true;
		}
	}

	OnQuestStatusUpdated.Broadcast(this);
}
