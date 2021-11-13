// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "QuestDialog.generated.h"

class AQuest;
class UQuestDescription;
class UButton;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	FSimpleDelegate OnQuestAccepted;
	FSimpleDelegate OnDialogFinished;

protected:
	UPROPERTY(meta=(BindWidget))
	UQuestDescription* Description;

	UPROPERTY(meta=(BindWidget))
	UButton* AcceptButton;

	UPROPERTY(meta=(BindWidget))
	UButton* RejectButton;

public:
	virtual void NativeConstruct() override;
	void Init(AQuest* Quest);

protected:
	UFUNCTION()
	void OnAccepted();

	UFUNCTION()
	void OnRejected();

	void HideDialog();
};
