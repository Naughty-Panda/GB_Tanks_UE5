#pragma once

#include "CoreMinimal.h"

#include "Quest.h"
#include "QuestObjective.h"

class SQuestTableRow : public SMultiColumnTableRow<AQuest*>
{
public:
	SLATE_BEGIN_ARGS(SQuestTableRow){}
	SLATE_ARGUMENT(AQuest*, Quest)
	SLATE_ARGUMENT(TSharedPtr<SBox>, ObjectivesHolder)
	SLATE_END_ARGS()

private:
	AQuest* Quest = nullptr;
	TSharedPtr<SBox> ObjectivesHolder;
	TSharedPtr<SListView<UQuestObjective*>> ObjectivesList;
	
public:
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

	// Constructor must have additional argument InOwnerTableView!
	// See Parent class implementation.
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);

private:
	FReply OpenObjectives();
	TSharedRef<ITableRow> GenerateObjectiveRow(UQuestObjective* Objective, const TSharedRef<STableViewBase>& Table);
	TSharedRef<SWidget> GetCreateObjectiveButton(EObjectiveType Type);
	FReply AddObjective(EObjectiveType Type);
};

class SObjectiveTableRow : public SMultiColumnTableRow<UQuestObjective*>
{
public:
	SLATE_BEGIN_ARGS(SObjectiveTableRow){}
	SLATE_ARGUMENT(AQuest*, Quest)
	SLATE_ARGUMENT(UQuestObjective*, Objective)
	SLATE_END_ARGS()

private:
	AQuest* Quest = nullptr;
	UQuestObjective* Objective = nullptr;
	STableViewBase* ParentTable = nullptr;

public:
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);

private:
	void RemoveObjective();
	void MoveUp();
	void MoveDown();
};
