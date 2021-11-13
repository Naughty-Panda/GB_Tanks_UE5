// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuestDescription.h"

#include "Quest.h"
#include "UI/ObjectiveWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UQuestDescription::Init(AQuest* Quest)
{
	if (!Quest)
	{
		return;
	}

	QuestNameText->SetText(Quest->GetQuestName());
	QuestDescriptionText->SetText(Quest->GetQuestDescription());

	if (ObjectiveWidgetClass)
	{
		ObjectivesList->ClearChildren();

		for (auto* Objective : Quest->GetObjectives())
		{
			auto* ObjectiveWidget = CreateWidget<UObjectiveWidget>(this, ObjectiveWidgetClass);
			ObjectiveWidget->Init(Objective);

			ObjectivesList->AddChild(ObjectiveWidget);
		}
	}
}
