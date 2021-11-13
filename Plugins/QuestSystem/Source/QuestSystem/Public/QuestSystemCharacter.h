// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "GameFramework/Character.h"

#include "QuestSystemCharacter.generated.h"

class UQuestDialog;

UCLASS()
class QUESTSYSTEM_API AQuestSystemCharacter : public ACharacter, public IInteractableObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Quest")
	TSubclassOf<UQuestDialog> QuestDialogClass;

public:
	// Sets default values for this character's properties
	AQuestSystemCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Interactable Interface class overrides.
	virtual void Interact_Implementation(AActor* InteractInstigator) override;
};
