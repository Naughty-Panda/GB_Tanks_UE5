#include "SQuestTableRow.h"

#include "ISinglePropertyView.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SWrapBox.h"

#include "QuestEditor.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SQuestTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	// Unreal's UPROPERTY module.
	// Helps to get and create a UPROPERTY() by name.
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;

	// PropertyModule.CreateSingleProperty() creates a default unreal widget in editor.
	if (InColumnName == "Name")
	{
		return SNew(SBox)
		[
			PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Name), Params).ToSharedRef()
		];
	}

	if (InColumnName == "Description")
	{
		return SNew(SBox)
		[
			PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Description), Params).ToSharedRef()
		];
	}

	if (InColumnName == "IsStory")
	{
		return SNew(SBox)
		[
			PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, bIsStoryQuest), Params).ToSharedRef()
		];
	}

	if (InColumnName == "Objectives")
	{
		return SNew(SBox)
		[
			SNew(SButton)
			.OnClicked_Raw(this, &SQuestTableRow::OpenObjectives)
			[
				SNew(SBox)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Show Objectives"))
				]
			]
		];
	}

	if (InColumnName == "KeepOrder")
	{
		return SNew(SBox)
		[
			PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, bKeepObjectivesOrder), Params).ToSharedRef()
		];
	}

	if (InColumnName == "PreviousQuest")
	{
		return SNew(SBox)
		[
			PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, PreviousQuest), Params).ToSharedRef()
		];
	}

	return SNew(SBox)[SNew(STextBlock).Text(FText::FromString("Wrong Quest Field!"))];
}

void SQuestTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Quest = InArgs._Quest;
	ObjectivesHolder = InArgs._ObjectivesHolder;

	FSuperRowType::Construct(FSuperRowType::FArguments(), InOwnerTableView);
}

FReply SQuestTableRow::OpenObjectives()
{
	ObjectivesHolder->SetContent(
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			SNew(SWrapBox)
			.Orientation(EOrientation::Orient_Horizontal)
			.PreferredWidth(256.f)
			+SWrapBox::Slot()[GetCreateObjectiveButton(EObjectiveType::OT_Interaction)]
			+SWrapBox::Slot()[GetCreateObjectiveButton(EObjectiveType::OT_Location)]
			+SWrapBox::Slot()[GetCreateObjectiveButton(EObjectiveType::OT_Collection)]
		]
		+SVerticalBox::Slot()
		[
			SAssignNew(ObjectivesList, SListView<UQuestObjective*>)
			.ItemHeight(24.f)
			.ListItemsSource(&Quest->GetObjectives())
			.OnGenerateRow_Raw(this, &SQuestTableRow::GenerateObjectiveRow)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Description")[FQuestEditorModule::GetRowHeader("Description")]
				+ SHeaderRow::Column("Type")[FQuestEditorModule::GetRowHeader("Type")]
				+ SHeaderRow::Column("Target")[FQuestEditorModule::GetRowHeader("Target")]
				+ SHeaderRow::Column("Delete")[FQuestEditorModule::GetRowHeader("Delete")]
				+ SHeaderRow::Column("Move")[FQuestEditorModule::GetRowHeader("Move")]
			)
		]
		);

	return FReply::Handled();
}

TSharedRef<ITableRow> SQuestTableRow::GenerateObjectiveRow(UQuestObjective* Objective, const TSharedRef<STableViewBase>& Table)
{
	return SNew(SObjectiveTableRow, Table)
	.Objective(Objective)
	.Quest(Quest);
}

TSharedRef<SWidget> SQuestTableRow::GetCreateObjectiveButton(EObjectiveType Type)
{
	FString TypeName;

	switch (Type)
	{
		case EObjectiveType::OT_Interaction: TypeName = "Interaction"; break;
		case EObjectiveType::OT_Location: TypeName="Location"; break;
		case EObjectiveType::OT_Collection: TypeName="Collection"; break;
	default: return SNullWidget::NullWidget;
	}

	return SNew(SButton)
	.OnClicked_Raw(this, &SQuestTableRow::AddObjective, Type)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Add " + TypeName + " objective"))
		];
}

FReply SQuestTableRow::AddObjective(EObjectiveType Type)
{
	switch (Type)
	{
	case EObjectiveType::OT_Interaction: Quest->AddInteractionObjective(); break;
	case EObjectiveType::OT_Location: Quest->AddLocationObjective(); break;
	case EObjectiveType::OT_Collection: Quest->AddCollectionObjective(); break;
	default: return FReply::Unhandled();
	}

	ObjectivesList->RequestListRefresh();
	return FReply::Handled();
}

/**
 * SObjectiveTableRow class methods.
 */

TSharedRef<SWidget> SObjectiveTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	
	// Unreal's UPROPERTY module.
	// Helps to get and create a UPROPERTY() by name.
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;

	// PropertyModule.CreateSingleProperty() creates a default unreal widget in editor.
	if (InColumnName == "Description")
	{
		return SNew(SBox)
		[
			PropertyModule.CreateSingleProperty(Objective, GET_MEMBER_NAME_CHECKED(UQuestObjective, Description), Params).ToSharedRef()
		];
	}

	if (InColumnName == "Type")
	{
		return SNew(SBox)
		.IsEnabled(false)
		[
			PropertyModule.CreateSingleProperty(Objective, GET_MEMBER_NAME_CHECKED(UQuestObjective, ObjectiveType), Params).ToSharedRef()
		];
	}

	if (InColumnName == "Target")
	{
		return SNew(SBox)
		.IsEnabled(false)
		[
			PropertyModule.CreateSingleProperty(Objective, Objective->GetWorldTargetName(), Params).ToSharedRef()
		];
	}

	if (InColumnName == "Delete")
	{
		return SNew(SBox)
		[
			SNew(SButton)
			.OnPressed_Raw(this, &SObjectiveTableRow::RemoveObjective)
			[
				SNew(SBox)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Delete objective"))
				]
			]
		];
	}

	if (InColumnName == "Move")
	{
		return SNew(SBox)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(SButton)
				.OnPressed_Raw(this, &SObjectiveTableRow::MoveUp)
				.IsEnabled(Quest->GetObjectives()[0] != Objective)
				[
					SNew(SBox)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Up"))
					]
				]
			]
			+SHorizontalBox::Slot()
			[
				SNew(SButton)
				.OnPressed_Raw(this, &SObjectiveTableRow::MoveDown)
				.IsEnabled(Quest->GetObjectives().Last() != Objective)
				[
					SNew(SBox)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Down"))
					]
				]
			]
		];
	}

	return SNew(SBox)[SNew(STextBlock).Text(FText::FromString("Wrong Quest Field!"))];
}

void SObjectiveTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Objective = InArgs._Objective;
	Quest = InArgs._Quest;

	FSuperRowType::Construct(FSuperRowType::FArguments(), InOwnerTableView);
	ParentTable = &InOwnerTableView.Get();
}

void SObjectiveTableRow::RemoveObjective()
{
	// TODO: Should not use const_cast! Add required Setter to TArray<OQuestObjectives*> Objectives!
	const_cast<TArray<UQuestObjective*>&>(Quest->GetObjectives()).Remove(Objective);

	// Notify UWorld about Quest changed, so it can properly register it for save system.
	Quest->Modify();

	ParentTable->RequestListRefresh();
}

void SObjectiveTableRow::MoveUp()
{
	const int32 Index = Quest->GetObjectives().Find(Objective);
	
	if (Index == 0)
	{
		return;
	}

	// TODO: Should not use const_cast! Add required Setter to TArray<OQuestObjectives*> Objectives!
	const_cast<TArray<UQuestObjective*>&>(Quest->GetObjectives()).Swap(Index, Index - 1);
	Quest->Modify();

	// Unlike RequestListRefresh() witch will rebuild list only if it was changed,
	// RebuildList() will always rebuild its list!
	ParentTable->RebuildList();
}

void SObjectiveTableRow::MoveDown()
{
	const int32 Index = Quest->GetObjectives().Find(Objective);
	
	if (Index == Quest->GetObjectives().Num() - 1)
	{
		return;
	}

	// TODO: Should not use const_cast! Add required Setter to TArray<OQuestObjectives*> Objectives!
	const_cast<TArray<UQuestObjective*>&>(Quest->GetObjectives()).Swap(Index, Index + 1);
	Quest->Modify();

	// Unlike RequestListRefresh() witch will rebuild list only if it was changed,
	// RebuildList() will always rebuild its list!
	ParentTable->RebuildList();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
