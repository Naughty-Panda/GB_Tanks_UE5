// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "QuestObjective.generated.h"

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	OT_Location,
	OT_Interaction,
	OT_Collection
};

/**
 * Quest Objective.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UQuestObjective*, QuestObjective);

UCLASS(Abstract)
class QUESTSYSTEM_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	EObjectiveType ObjectiveType;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsCompleted :1;
	UPROPERTY(VisibleAnywhere)
	uint8 bCanBeCompleted :1;

public:
	FOnObjectiveCompleted OnObjectiveCompleted;

public:
	virtual void ActivateObjective(AActor* Instigator) {}
};

/**
 * Interaction Objective.
 */

UCLASS()
class QUESTSYSTEM_API UInteractionObjective : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(AllowedClasses = "InteractableObject"))
	AActor* Target;

public:
	UInteractionObjective();

public:
	virtual void ActivateObjective(AActor* Instigator) override;

private:
	UFUNCTION()
	void Internal_CompleteObjective(AActor* Object, AActor* Instigator);
};

/**
 * Location Marker Objective.
 */

UCLASS()
class QUESTSYSTEM_API ULocationObjective : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(AllowedClasses = "LocationMarkerActor"))
	AActor* Marker;

public:
	ULocationObjective();

public:
	virtual void ActivateObjective(AActor* Instigator) override;
};

/**
 * Item Collection Objective.
 */

UCLASS()
class QUESTSYSTEM_API UCollectionObjective : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	AActor* ItemToCollect;

	UPROPERTY(EditAnywhere)
	int32 ItemRequiredID;

	UPROPERTY(EditAnywhere)
	int32 ItemCurrentCount;

	UPROPERTY(EditAnywhere)
	int32 ItemRequiredCount;

public:
	UCollectionObjective();

public:
	virtual void ActivateObjective(AActor* Instigator) override;
};
