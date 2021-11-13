// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuestDialog.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/QuestDescription.h"

void UQuestDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (AcceptButton)
	{
		AcceptButton->OnReleased.AddDynamic(this, &ThisClass::UQuestDialog::OnAccepted);
	}

	if (RejectButton)
	{
		RejectButton->OnReleased.AddDynamic(this, &ThisClass::UQuestDialog::OnRejected);
	}

	if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, this);
		PlayerController->SetShowMouseCursor(true);
	}
}

void UQuestDialog::Init(AQuest* Quest)
{
	if (Description)
	{
		Description->Init(Quest);
	}
}

void UQuestDialog::OnAccepted()
{
	HideDialog();

	std::ignore = OnQuestAccepted.ExecuteIfBound();
}

void UQuestDialog::OnRejected()
{
	HideDialog();
}

void UQuestDialog::HideDialog()
{
	if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		PlayerController->SetShowMouseCursor(true);
	}

	RemoveFromViewport();
	std::ignore = OnDialogFinished.ExecuteIfBound();
}
