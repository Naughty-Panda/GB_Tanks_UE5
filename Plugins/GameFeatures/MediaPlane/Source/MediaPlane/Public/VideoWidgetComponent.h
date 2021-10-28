// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "VideoWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup="UserInterface", meta=(BlueprintSpawnableComponent))
class MEDIAPLANE_API UVideoWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
