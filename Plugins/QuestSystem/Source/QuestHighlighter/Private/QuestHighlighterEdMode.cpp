// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestHighlighterEdMode.h"
#include "QuestHighlighterEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "Selection.h"

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
	for (AActor* Actor : SelectedActors)
	{
		// Draw Box.
		DrawWireBox(PDI, Actor->GetComponentsBoundingBox(true), FColor::Yellow, 1);

		// Draw Sphere.
		const float SphereRadius = (Actor->GetComponentsBoundingBox(true).GetCenter() - Actor->GetComponentsBoundingBox().Max).Size();
		DrawWireSphere(PDI, Actor->GetComponentsBoundingBox(true).GetCenter(), FColor::Cyan, SphereRadius, 32, 1);
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

	for (AActor* Actor : SelectedActors)
	{
		const FBox ActorBounds = Actor->GetComponentsBoundingBox(true);

		const FVector DrawLocation = ActorBounds.GetCenter() + FVector(0.f, 0.f, ActorBounds.GetExtent().Z);

		FVector2D DrawPixel;

		View->WorldToPixel(DrawLocation, DrawPixel);

		// Adjust render to DPI scale.
		DrawPixel /= ViewportClient->GetDPIScale();

		const FText DrawText = FText::FromString(Actor->GetName());

		Canvas->DrawShadowedText(DrawPixel.X, DrawPixel.Y, DrawText, GEngine->GetSmallFont(), FColor::Yellow);
	}
}

void FQuestHighlighterEdMode::ActorSelectionChangeNotify()
{
	FEdMode::ActorSelectionChangeNotify();

	UpdateSelectedActors();
}

void FQuestHighlighterEdMode::UpdateSelectedActors()
{
	SelectedActors.Empty();

	USelection* Selection = GEditor->GetSelectedActors();

	// Making Iterator from Selection.
	for (FSelectionIterator Iter(*Selection); Iter; ++Iter)
	{
		if (AActor* SelectedActor = Cast<AActor>(*Iter))
		{
			SelectedActors.AddUnique(SelectedActor);
		}
	}
}
