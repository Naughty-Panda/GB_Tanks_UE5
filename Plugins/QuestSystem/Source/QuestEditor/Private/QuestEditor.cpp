// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestEditor.h"
#include "QuestEditorStyle.h"
#include "QuestEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

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
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FQuestEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("QuestEditor.cpp"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FQuestEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuestEditorTabName);
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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuestEditorModule, QuestEditor)
