// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class UQuestObjective;
class AQuestSystemCharacter;

class FQuestHighlighterEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_QuestHighlighterEdModeId;

	FLinearColor NPCColor = FLinearColor::Yellow;
	FLinearColor ObjectivesColor = FLinearColor::Blue;
	float ObjectivesSphereRadius = 128.f;

protected:
	TArray<AQuestSystemCharacter*> SelectedNPCs;
	TArray<UQuestObjective*> SelectedObjectives;

public:
	FQuestHighlighterEdMode();
	virtual ~FQuestHighlighterEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual void ActorSelectionChangeNotify() override;

protected:
	void UpdateSelectedActors();
};
