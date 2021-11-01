// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"

#include "InteractableObject.h"
#include "Blueprint/UserWidget.h"

UInteractionComponent::UInteractionComponent()
{
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnEndOverlap);
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick Tick, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);
}

void UInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UInteractableObject>())
	{
		return;
	}

	ActorToInteract = OtherActor;

	if (InteractMessageWidget == nullptr && InteractMessageWidgetClass)
	{
		InteractMessageWidget = CreateWidget(GetWorld(), InteractMessageWidgetClass);
	}

	if (InteractMessageWidget && !InteractMessageWidget->IsInViewport())
	{
		InteractMessageWidget->AddToViewport();
	}
}

void UInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ActorToInteract == OtherActor)
	{
		ActorToInteract = nullptr;

		if (InteractMessageWidget)
		{
			InteractMessageWidget->RemoveFromViewport();
		}
	}
}

void UInteractionComponent::Interact()
{
	if (ActorToInteract)
	{
		// Same as:
		// Cast<IInteractableObject>(ActorToInteract)->Interact(GetOwner());
		// But using internal IInteractableObject method without casting.
		IInteractableObject::Execute_Interact(ActorToInteract, GetOwner());
	}
}
