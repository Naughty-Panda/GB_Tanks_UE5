// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS(ClassGroup=(Collision), meta=(BlueprintSpawnableComponent), DisplayName="Interaction Collision")
class QUESTSYSTEM_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InteractMessageWidgetClass;

	UPROPERTY()
	UUserWidget* InteractMessageWidget;

	UPROPERTY()
	AActor* ActorToInteract;

public:
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// OnComponentBeginOverlap
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// OnComponentEndOverlap
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Interact();
};
