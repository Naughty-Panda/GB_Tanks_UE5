// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuestLogWidget.h"

#include "Quest.h"
#include "QuestListComponent.h"
#include "Components/ScrollBox.h"
#include "UI/QuestDescription.h"
#include "UI/QuestLogEntry.h"

void UQuestLogWidget::Init(UQuestListComponent* QuestList)
{
	if (QuestList && QuestLogEntryClass)
	{
		for (auto* Quest : QuestList->GetQuests())
		{
			UScrollBox* ScrollBox = Quest->bIsStoryQuest ? StoryQuestList : SideQuestList;

			if (UQuestLogEntry* QuestEntry = CreateWidget<UQuestLogEntry>(this, QuestLogEntryClass))
			{
				QuestEntry->SetQuestText(Quest->GetQuestName());
				QuestEntry->OnQuestSelected.BindUObject(QuestList, &UQuestListComponent::SetActiveQuest, Quest);
				QuestEntries.Add(Quest, QuestEntry);
				ScrollBox->AddChild(QuestEntry);
			}
		}

		OnActiveQuestChanged(QuestList->GetActiveQuest());
		QuestList->OnActiveQuestChanged.AddUObject(this, &UQuestLogWidget::OnActiveQuestChanged);
	}
}

void UQuestLogWidget::OnActiveQuestChanged(AQuest* ActiveQuest)
{
	if (ActiveQuest)
	{
		for (const auto& QuestEntry : QuestEntries)
		{
			QuestEntry.Value->SetIsSelected(QuestEntry.Key == ActiveQuest);
		}

		if (ActiveQuestDescription)
		{
			ActiveQuestDescription->Init(ActiveQuest);
			ActiveQuestDescription->SetVisibility(ActiveQuest ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
}
