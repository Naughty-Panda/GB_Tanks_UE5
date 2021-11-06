// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestObjective.h"

#include "CollectableItemInterface.h"
#include "InteractableObject.h"
#include "LocationMarkerInterface.h"

/**
 * Interaction Objective.
 */

UInteractionObjective::UInteractionObjective()
{
	ObjectiveType = EObjectiveType::OT_Interaction;
}

void UInteractionObjective::ActivateObjective(AActor* Instigator)
{
	if (auto* InteractTarget = Cast<IInteractableObject>(Target))
	{
		InteractTarget->OnInteractionFinished.AddDynamic(this, &UInteractionObjective::Internal_CompleteObjective);
	}
}

void UInteractionObjective::Internal_CompleteObjective(AActor* Object, AActor* Instigator)
{
	// Check here.
	if (bCanBeCompleted && Instigator == Target)
	{
		bIsCompleted = true;
		OnObjectiveCompleted.Broadcast(this);
	}
}

/**
 * Location Marker Objective.
 */

ULocationObjective::ULocationObjective()
{
	ObjectiveType = EObjectiveType::OT_Location;
}

void ULocationObjective::ActivateObjective(AActor* Instigator)
{
	if (auto* LocationMarker = Cast<ILocationMarkerInterface>(Marker))
	{
		LocationMarker->OnLocationReached.AddLambda([this, &Instigator](AActor* Marker, AActor* InteractInstigator)
		{
			if (bCanBeCompleted && Instigator == InteractInstigator)
			{
				bIsCompleted = true;
				OnObjectiveCompleted.Broadcast(this);
			}
		});
	}
}

/**
 * Item Collection Objective.
 */

UCollectionObjective::UCollectionObjective()
{
	ObjectiveType = EObjectiveType::OT_Collection;
}

void UCollectionObjective::ActivateObjective(AActor* Instigator)
{
	if (auto* CollectableItem = Cast<ICollectableItemInterface>(ItemToCollect))
	{
		CollectableItem->OnItemCollected.AddLambda([this](int32 ItemID, int32 ItemCount)
		{
			if (bCanBeCompleted && ItemID == ItemRequiredID)
			{
				ItemCurrentCount += ItemCount;
				if (ItemCurrentCount >= ItemRequiredCount)
				{
					bIsCompleted = true;
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}
}
