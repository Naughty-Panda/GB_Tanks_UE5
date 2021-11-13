// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuestLogEntry.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UQuestLogEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (QuestSelectButton)
	{
		QuestSelectButton->OnReleased.AddDynamic(this, &ThisClass::UQuestLogEntry::SelectQuest);
	}
}

void UQuestLogEntry::SetQuestText(const FText& inQuestText)
{
	if (QuestText)
	{
		QuestText->SetText(inQuestText);
	}
}

void UQuestLogEntry::SetIsSelected(bool bIsSelected)
{
	if (SelectionBorder)
	{
		SelectionBorder->SetBrushColor(bIsSelected ? FLinearColor::White : FLinearColor(1.f, 1.f, 1.f, 0.f));
	}
}

void UQuestLogEntry::SelectQuest()
{
	std::ignore = OnQuestSelected.ExecuteIfBound();
}
