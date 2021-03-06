// Fill out your copyright notice in the Description page of Project Settings.

#include "UIManager.h"
#include "MinimapUMGWidget.h"
#include "../../../Plugins/QuestSystem/Source/QuestSystem/Public/UI/QuestLogWidget.h"
#include "Blueprint/UserWidget.h"
#include "GB_Tanks_UE5/TankPawn.h"

AUIManager::AUIManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUIManager::BeginPlay()
{
	Super::BeginPlay();

	// Draw Minimap widget on BeginPlay.
	SetActiveWidget(EWidgetType::WT_Minimap);
}

void AUIManager::SetActiveWidget(const EWidgetType WidgetType, const int32 ZOrder)
{
	if (ActiveWidgetType == WidgetType)
	{
		return;
	}

	if (const TSubclassOf<UUserWidget>* WidgetClass = WidgetTypes.Find(WidgetType))
	{
		AddWidgetByClass(*WidgetClass, ZOrder);
		ActiveWidgetType = WidgetType;
	}
}

void AUIManager::ToggleQuestLogVisibility()
{
	// If Quest Log exists - remove it and return.
	if (QuestLog)
	{
		QuestLog->RemoveFromParent();
		QuestLog = nullptr;
		return;
	}

	// If there is no Quest Log - Create new widget and add it to viewport.
	if (QuestLogWidgetClass)
	{
		QuestLog = CreateWidget<UQuestLogWidget>(GetOwningPlayerController(), QuestLogWidgetClass);
		QuestLog->Init(Cast<ATankPawn>(GetOwningPawn())->GetQuestListComponent());
		QuestLog->AddToViewport();
	}
}

void AUIManager::AddWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder)
{
	RemoveActiveWidget();

	ActiveWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), WidgetClass);
	if (ActiveWidget)
	{
		ActiveWidget->AddToViewport(ZOrder);
	}
}

void AUIManager::RemoveActiveWidget()
{
	if (ActiveWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;

		ActiveWidgetType = EWidgetType::WT_None;
	}
}

void AUIManager::UpdatePlayerPositionOnMinimap(const FVector2D& InPosition)
{
	if (ActiveWidgetType == EWidgetType::WT_Minimap)
	{
		if (UMinimapUMGWidget* MinimapWidget = Cast<UMinimapUMGWidget>(ActiveWidget))
		{
			MinimapWidget->UpdateMinimap(InPosition);
		}
	}
}
