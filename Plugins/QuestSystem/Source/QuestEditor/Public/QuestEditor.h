// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class AQuest;

class FQuestEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	void UpdateQuests();
	FReply UpdateQuestList();
	void TabClosed(TSharedRef<SDockTab> Tab);
	static TSharedRef<SWidget> GetRowHeader(const FString& Text);

private:
	void RegisterMenus();

	// Adds menu extension.
	void AddMenuExtension(FMenuBuilder& MenuBuilder);
	// Adds Toolbar Extension
	void AddToolbarExtension(FToolBarBuilder& ToolBarBuilder);

	TSharedRef<ITableRow> GenerateQuestRow(AQuest* Quest, const TSharedRef<STableViewBase>& Table);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TArray<AQuest*> Quests;

	TSharedPtr<SListView<AQuest*>> QuestList;

	TSharedPtr<SBox> ObjectivesHolder;
};
