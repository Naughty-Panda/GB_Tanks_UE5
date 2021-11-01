// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

// This class does not need to be modified.
// Blueprint class.
UINTERFACE(MinimalAPI)
class UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished, AActor*, InteractObject, AActor*, InteractInstigator);

// C++ class.
class QUESTSYSTEM_API IInteractableObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor* InteractInstigator);
	virtual void Interact_Implementation(AActor* InteractInstigator) {}

	virtual void NotifyInteractionFinished(AActor* InteractObject, AActor* InteractInstigator);

	FOnInteractionFinished OnInteractionFinished;
};
