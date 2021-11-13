// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ObjectiveWidget.h"

#include "QuestObjective.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

void UObjectiveWidget::Init(UQuestObjective* Objective)
{
	if (!Objective)
	{
		return;
	}

	ObjectiveStatusBox->SetIsChecked(false);
	ObjectiveDescriptionText->SetText(Objective->GetDescription());
}
