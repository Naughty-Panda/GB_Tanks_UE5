// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestHighlighterEdMode.h"
#include "QuestHighlighterEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "Selection.h"
#include "EngineUtils.h"

// QuestSystem includes
#include "Quest.h"
#include "QuestSystemCharacter.h"
#include "QuestObjective.h"

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

void FQuestHighlighterEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	for (AQuestSystemCharacter* NPC : SelectedNPCs)
	{
		// Draw Box.
		DrawWireBox(PDI, NPC->GetComponentsBoundingBox(true), NPCColor, 1);
	}

	for (UQuestObjective* Objective : SelectedObjectives)
	{
		// Draw Sphere.
		const FVector SphereCenter = Objective->GetObjectiveTarget()->GetComponentsBoundingBox(true).GetCenter();
		DrawWireSphere(PDI, SphereCenter, ObjectivesColor, ObjectivesSphereRadius, 32, 1);
	}

	FEdMode::Render(View, Viewport, PDI);
}

void FQuestHighlighterEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);

	if (!Canvas)
	{
		return;
	}

	// Drawing HUD for NPCs.
	for (AQuestSystemCharacter* NPC : SelectedNPCs)
	{
		const FBox ActorBounds = NPC->GetComponentsBoundingBox(true);

		const FVector DrawLocation = ActorBounds.GetCenter() + FVector(0.f, 0.f, ActorBounds.GetExtent().Z);

		FVector2D DrawPixel;

		View->WorldToPixel(DrawLocation, DrawPixel);

		// Adjust render to DPI scale.
		DrawPixel /= ViewportClient->GetDPIScale();

		const FText DrawText = FText::FromString(NPC->GetName());

		Canvas->DrawShadowedText(DrawPixel.X, DrawPixel.Y, DrawText, GEngine->GetSmallFont(), NPCColor);
	}

	// Drawing HUD for Objectives.
	for (UQuestObjective* Objective : SelectedObjectives)
	{
		const FBox ObjectiveBounds = Objective->GetObjectiveTarget()->GetComponentsBoundingBox(true);

		const FVector DrawLocation = ObjectiveBounds.GetCenter() + FVector(0.f, 0.f, ObjectiveBounds.GetExtent().Z);

		FVector2D DrawPixel;

		View->WorldToPixel(DrawLocation, DrawPixel);

		// Adjust render to DPI scale.
		DrawPixel /= ViewportClient->GetDPIScale();

		const FText DrawText = FText::FromString(Objective->GetName());

		Canvas->DrawShadowedText(DrawPixel.X, DrawPixel.Y, DrawText, GEngine->GetSmallFont(), ObjectivesColor);
	}
}

void FQuestHighlighterEdMode::ActorSelectionChangeNotify()
{
	FEdMode::ActorSelectionChangeNotify();

	UpdateSelectedActors();
}

void FQuestHighlighterEdMode::UpdateSelectedActors()
{
	SelectedNPCs.Empty();
	SelectedObjectives.Empty();

	USelection* Selection = GEditor->GetSelectedActors();

	// Making Iterator from Selection.
	for (FSelectionIterator Iter(*Selection); Iter; ++Iter)
	{
		if (AQuestSystemCharacter* NPC = Cast<AQuestSystemCharacter>(*Iter))
		{
			SelectedNPCs.AddUnique(NPC);
		}
	}

	if (!SelectedNPCs.IsEmpty())
	{
		for (auto* NPC : SelectedNPCs)
		{
			TArray<AActor*> AttachedActors;
			NPC->GetAttachedActors(AttachedActors);

			for (AActor* Actor : AttachedActors)
			{
				if (AQuest* Quest = Cast<AQuest>(Actor))
				{
					SelectedObjectives.Append(Quest->GetObjectives());
				}
			}
		}

		return;
	}

	// Get all NPCs in current world.
	auto* CurrentWorld = GEditor->GetEditorWorldContext().World();
	for (TActorIterator<AQuestSystemCharacter> Iter(CurrentWorld, AQuestSystemCharacter::StaticClass()); Iter; ++Iter)
	{
		SelectedNPCs.AddUnique(*Iter);
	}
}
