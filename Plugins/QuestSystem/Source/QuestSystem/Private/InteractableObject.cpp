// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"

// Add default functionality here for any IInteractableObject functions that are not pure virtual.
void IInteractableObject::NotifyInteractionFinished(AActor* InteractObject, AActor* InteractInstigator)
{
	OnInteractionFinished.Broadcast(InteractObject, InteractInstigator);
}
