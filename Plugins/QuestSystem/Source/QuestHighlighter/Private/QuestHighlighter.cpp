// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestHighlighter.h"
#include "QuestHighlighterEdMode.h"

#define LOCTEXT_NAMESPACE "FQuestHighlighterModule"

void FQuestHighlighterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FQuestHighlighterEdMode>(FQuestHighlighterEdMode::EM_QuestHighlighterEdModeId, LOCTEXT("QuestHighlighterEdModeName", "QuestHighlighterEdMode"), FSlateIcon(), true);
}

void FQuestHighlighterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FQuestHighlighterEdMode::EM_QuestHighlighterEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestHighlighterModule, QuestHighlighter)
