// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystemCharacter.h"

#include "QuestListComponent.h"
#include "Quest.h"
#include "UI/QuestDialog.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AQuestSystemCharacter::AQuestSystemCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called to bind functionality to input
void AQuestSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AQuestSystemCharacter::Interact_Implementation(AActor* InteractInstigator)
{
	if (!InteractInstigator)
	{
		return;
	}

	if (UQuestListComponent* QuestList = Cast<UQuestListComponent>(InteractInstigator->GetComponentByClass(UQuestListComponent::StaticClass())))
	{
		TArray<AActor*> AttachedActors;
		GetAttachedActors(AttachedActors);

		for (const auto& Actor : AttachedActors)
		{
			if (AQuest* Quest = Cast<AQuest>(Actor))
			{
				if (Quest->bIsTaken || (Quest->GetPreviousQuest() && !Quest->GetPreviousQuest()->IsCompleted()))
				{
					continue;
				}

				if (QuestDialogClass)
				{
					if (UQuestDialog* QuestDialog = CreateWidget<UQuestDialog>(GetWorld(), QuestDialogClass))
					{
						QuestDialog->Init(Quest);
						QuestDialog->OnQuestAccepted.BindUObject(QuestList, &UQuestListComponent::AddQuest, Quest);
						QuestDialog->OnDialogFinished.BindLambda([this, &InteractInstigator]()
						{
							NotifyInteractionFinished(this, InteractInstigator);
						});
						QuestDialog->AddToViewport();

						return;
					}
				}
			}
		}
	}

	NotifyInteractionFinished(this, InteractInstigator);
}
