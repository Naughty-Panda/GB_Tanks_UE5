// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentReceiver.h"
#include "../../../../../Unreal Engine/UE_5.0EA/Engine/Plugins/Experimental/ModularGameplay/Source/ModularGameplay/Public/Components/GameFrameworkComponentManager.h"

// Add default functionality here for any IComponentReceiver functions that are not pure virtual.
void IComponentReceiver::RegisterAsComponentReceiver(AActor* InActor) const
{
	if (UGameFrameworkComponentManager* ComponentManager = InActor->GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>())
	{
		ComponentManager->AddReceiver(InActor);
	}
}
