// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniMapWidget.h"
#include "Blueprint/UserWidget.h"

#include "MinimapUMGWidget.generated.h"

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API UMinimapUMGWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMiniMapWidget* Minimap;

	void UpdateMinimap(const FVector2D& InPlayerPosition);
};
