// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestEditor.h"
#include "QuestEditorStyle.h"
#include "QuestEditorCommands.h"
#include "LevelEditor.h"
#include "Selection.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

// Quest System includes
#include "Quest.h"
#include "SQuestTableRow.h"

static const FName QuestEditorTabName("QuestEditor");

#define LOCTEXT_NAMESPACE "FQuestEditorModule"

void FQuestEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FQuestEditorStyle::Initialize();
	FQuestEditorStyle::ReloadTextures();

	FQuestEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FQuestEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FQuestEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FQuestEditorModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuestEditorTabName, FOnSpawnTab::CreateRaw(this, &FQuestEditorModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FQuestEditorTabTitle", "QuestEditor"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);

	// End of Unreal-generated code.

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	// Create Menu Extension.
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands,
	                               FMenuExtensionDelegate::CreateRaw(this, &FQuestEditorModule::AddMenuExtension));

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	// Create Toolbar extension.
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("EditorModes", EExtensionHook::After, PluginCommands,
	                                     FToolBarExtensionDelegate::CreateRaw(this, &FQuestEditorModule::AddToolbarExtension));

	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FQuestEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FQuestEditorStyle::Shutdown();

	FQuestEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuestEditorTabName);
}

TSharedRef<SDockTab> FQuestEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	/*FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FQuestEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("QuestEditor.cpp"))
	);*/

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.OnTabClosed_Raw(this, &FQuestEditorModule::TabClosed)
		[
			// Put your tab content here!
			SNew(SBox)
			//.HAlign(HAlign_Center)
			//.VAlign(VAlign_Center)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
					.OnClicked_Raw(this, &FQuestEditorModule::UpdateQuestList)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Update quest list from selection"))
					]
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SNew(SBox)
					[
					SAssignNew(QuestList, SListView<AQuest*>)
					.ItemHeight(24.f)
					.ListItemsSource(&Quests)
					.OnGenerateRow_Raw(this, &FQuestEditorModule::GenerateQuestRow)
					.HeaderRow
					(
						SNew(SHeaderRow)
						+SHeaderRow::Column("Name")[GetRowHeader("Name")]
						+SHeaderRow::Column("Description")[GetRowHeader("Description")]
						+SHeaderRow::Column("IsStory")[GetRowHeader("Is Story")]
						+SHeaderRow::Column("Objectives")[GetRowHeader("Objectives")]
						+SHeaderRow::Column("KeepOrder")[GetRowHeader("Keep Order")]
						+SHeaderRow::Column("PreviousQuest")[GetRowHeader("Previous Quest")]
						)
					]
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SAssignNew(ObjectivesHolder, SBox)
				]
			]
		];
}

void FQuestEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuestEditorTabName);
}

void FQuestEditorModule::UpdateQuests()
{
	Quests.Empty();

	// Find available quests to select.
	for (FSelectionIterator Iter(*GEditor->GetSelectedActors()); Iter; ++Iter)
	{
		if (auto* Quest = Cast<AQuest>(*Iter))
		{
			Quests.AddUnique(Quest);
		}
	}

	// Select those quests.
	if (!Quests.IsEmpty())
	{
		GEditor->SelectNone(true, true);

		for (auto* Quest : Quests)
		{
			GEditor->SelectActor(Quest, true, false);
		}
	}
}

FReply FQuestEditorModule::UpdateQuestList()
{
	// Clean Objectives widget.
	ObjectivesHolder->SetContent(SNullWidget::NullWidget);
	UpdateQuests();

	// Does nothing if List doesn't change.
	QuestList->RequestListRefresh();

	return FReply::Handled();
}

void FQuestEditorModule::TabClosed(TSharedRef<SDockTab> Tab)
{
	ObjectivesHolder.Reset();
	QuestList.Reset();
}

void FQuestEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	// We replaced this with our code in StartupModule().
	// Menu Extender.
	/*
		{
			UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
			{
				FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
				Section.AddMenuEntryWithCommandList(FQuestEditorCommands::Get().OpenPluginWindow, PluginCommands);
			}
		}
	*/

	// Toolbar Extender.
	/*
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FQuestEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
	*/
}

void FQuestEditorModule::AddMenuExtension(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(FQuestEditorCommands::Get().OpenPluginWindow);
}

void FQuestEditorModule::AddToolbarExtension(FToolBarBuilder& ToolBarBuilder)
{
	ToolBarBuilder.AddToolBarButton(FQuestEditorCommands::Get().OpenPluginWindow);
}

TSharedRef<ITableRow> FQuestEditorModule::GenerateQuestRow(AQuest* Quest, const TSharedRef<STableViewBase>& Table)
{
	return SNew(SQuestTableRow, Table)
	.Quest(Quest)
	.ObjectivesHolder(ObjectivesHolder);
}

TSharedRef<SWidget> FQuestEditorModule::GetRowHeader(const FString& Text)
{
	return SNew(SBorder)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.Padding(1.f)
	.Content()
	[
		SNew(STextBlock)
		.Text(FText::FromString(Text))
	];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuestEditorModule, QuestEditor)
