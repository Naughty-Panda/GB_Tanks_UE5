// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestListComponent.h"

#include "Quest.h"
#include "UI/CurrentObjectives.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UQuestListComponent::UQuestListComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UQuestListComponent::BeginPlay()
{
	Super::BeginPlay();

	// Creating CurrentObjectiveWidget.
	if (!CurrentObjectivesWidgetClass)
	{
		return;
	}

	auto* CurrentObjectivesWidget = CreateWidget<UCurrentObjectives>(GetWorld(), CurrentObjectivesWidgetClass);
	OnActiveQuestChanged.AddUObject(CurrentObjectivesWidget, &UCurrentObjectives::SetCurrentObjectives);
	CurrentObjectivesWidget->AddToViewport();
}


// Called every frame
void UQuestListComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestListComponent::AddQuest(AQuest* Quest)
{
	if (!Quest)
	{
		return;
	}

	if (!ActiveQuest || ActiveQuest->IsCompleted())
	{
		ActiveQuest = Quest;
		OnActiveQuestChanged.Broadcast(ActiveQuest);
	}

	AcceptedQuests.AddUnique(Quest);
	Quest->TakeQuest(GetOwner());

	// TODO: Check here
	Quest->OnQuestStatusUpdated.AddLambda([this](AQuest* ChangedQuest)
	{
		if (ActiveQuest == ChangedQuest)
		{
			OnActiveQuestChanged.Broadcast(ChangedQuest);
		}
	});
}

const TArray<AQuest*>& UQuestListComponent::GetQuests() const
{
	return AcceptedQuests;
}

AQuest* UQuestListComponent::GetActiveQuest() const
{
	return ActiveQuest;
}

void UQuestListComponent::SetActiveQuest(AQuest* Quest)
{
	if (AcceptedQuests.Contains(Quest))
	{
		ActiveQuest = Quest;
		OnActiveQuestChanged.Broadcast(ActiveQuest);
	}
}
