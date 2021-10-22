// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "TanksDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API UTanksDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	FText TankName;
};
