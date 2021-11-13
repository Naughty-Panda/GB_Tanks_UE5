// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CurrentObjectives.h"

#include "Quest.h"
#include "UI/ObjectiveWidget.h"
#include "Components/VerticalBox.h"

void UCurrentObjectives::SetCurrentObjectives(AQuest* Quest)
{
	if (!ObjectivesList || !ObjectiveWidgetClass)
	{
		return;
	}

	ObjectivesList->ClearChildren();

	for (auto* Objective : Quest->GetObjectives())
	{
		auto* ObjectiveWidget = CreateWidget<UObjectiveWidget>(this, ObjectiveWidgetClass);
		ObjectiveWidget->Init(Objective);

		auto* VerticalBoxSlot = ObjectivesList->AddChildToVerticalBox(ObjectiveWidget);
	}
}
