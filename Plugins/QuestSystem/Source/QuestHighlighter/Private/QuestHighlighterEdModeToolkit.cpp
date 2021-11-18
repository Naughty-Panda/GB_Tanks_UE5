// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestHighlighterEdModeToolkit.h"
#include "QuestHighlighterEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Colors/SColorWheel.h"

#define LOCTEXT_NAMESPACE "FQuestHighlighterEdModeToolkit"

FQuestHighlighterEdModeToolkit::FQuestHighlighterEdModeToolkit()
{
}

void FQuestHighlighterEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
				]

				// Our widgets.
				+SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.Padding(1.f, 10.f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.Text(LOCTEXT("NPCColor", "NPC Color:"))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.f, 0.f)
					[
						SNew(SColorBlock)
						.AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
						.Size(FVector2D(50.f,16.f))
						.Color_Raw(this, &FQuestHighlighterEdModeToolkit::GetNPCColor)
						.OnMouseButtonDown_Raw(this, &FQuestHighlighterEdModeToolkit::OnOpenNPCColor)
					]
				]
				+SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SColorWheel)
					.SelectedColor(this, &FQuestHighlighterEdModeToolkit::GetNPCColor)
					.OnValueChanged(this, &FQuestHighlighterEdModeToolkit::SetNPCColor)
				]
				
				+SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.Text(LOCTEXT("ObjectiveColor", "Objective Color:"))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.f, 0.f)
					[
						SNew(SColorBlock)
						.AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
						.Size(FVector2D(50.f,16.f))
						.Color_Raw(this, &FQuestHighlighterEdModeToolkit::GetObjectivesColor)
						.OnMouseButtonDown_Raw(this, &FQuestHighlighterEdModeToolkit::OnOpenObjectivesColor)
					]
				]
				+SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SColorWheel)
					.SelectedColor(this, &FQuestHighlighterEdModeToolkit::GetObjectivesColor)
					.OnValueChanged(this, &FQuestHighlighterEdModeToolkit::SetObjectivesColor)
				]
		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FQuestHighlighterEdModeToolkit::GetToolkitFName() const
{
	return FName("Quest Highlighter");
}

FText FQuestHighlighterEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("QuestHighlighterEdModeToolkit", "DisplayName", "QuestHighlighterEdMode Tool");
}

FLinearColor FQuestHighlighterEdModeToolkit::GetNPCColor() const
{
	const auto* Mode = StaticCast<FQuestHighlighterEdMode*>(GetEditorMode());
	return Mode->NPCColor;
}

void FQuestHighlighterEdModeToolkit::SetNPCColor(FLinearColor NewColor)
{
	auto* Mode = StaticCast<FQuestHighlighterEdMode*>(GetEditorMode());
	Mode->NPCColor = NewColor;
}

FLinearColor FQuestHighlighterEdModeToolkit::GetObjectivesColor() const
{
	const auto* Mode = StaticCast<FQuestHighlighterEdMode*>(GetEditorMode());
	return Mode->ObjectivesColor;
}

void FQuestHighlighterEdModeToolkit::SetObjectivesColor(FLinearColor NewColor)
{
	auto* Mode = StaticCast<FQuestHighlighterEdMode*>(GetEditorMode());
	Mode->ObjectivesColor = NewColor;
}

FReply FQuestHighlighterEdModeToolkit::OnOpenNPCColor(const FGeometry&, const FPointerEvent&)
{
	FColorPickerArgs PickerArgs;
	PickerArgs.bUseAlpha = false;
	
	// bIsModal = Open Picker in new window!
	// Instead use PickerArgs.ParentWidget
	PickerArgs.bIsModal = true;
	PickerArgs.InitialColorOverride = GetNPCColor();
	PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &FQuestHighlighterEdModeToolkit::SetNPCColor);

	OpenColorPicker(PickerArgs);

	return FReply::Handled();
}

FReply FQuestHighlighterEdModeToolkit::OnOpenObjectivesColor(const FGeometry&, const FPointerEvent&)
{
	FColorPickerArgs PickerArgs;
	PickerArgs.bUseAlpha = false;
	PickerArgs.bIsModal = true;
	PickerArgs.InitialColorOverride = GetObjectivesColor();
	PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &FQuestHighlighterEdModeToolkit::SetObjectivesColor);

	OpenColorPicker(PickerArgs);

	return FReply::Handled();
}

class FEdMode* FQuestHighlighterEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FQuestHighlighterEdMode::EM_QuestHighlighterEdModeId);
}

#undef LOCTEXT_NAMESPACE
