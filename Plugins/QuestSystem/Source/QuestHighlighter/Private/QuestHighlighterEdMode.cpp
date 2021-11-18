// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestHighlighterEdMode.h"
#include "QuestHighlighterEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FQuestHighlighterEdMode::EM_QuestHighlighterEdModeId = TEXT("EM_QuestHighlighterEdMode");

FQuestHighlighterEdMode::FQuestHighlighterEdMode()
{

}

FQuestHighlighterEdMode::~FQuestHighlighterEdMode()
{

}

void FQuestHighlighterEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FQuestHighlighterEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FQuestHighlighterEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FQuestHighlighterEdMode::UsesToolkits() const
{
	return true;
}




