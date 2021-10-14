// Fill out your copyright notice in the Description page of Project Settings.

#include "MinimapUMGWidget.h"

void UMinimapUMGWidget::UpdateMinimap(const FVector2D& InPlayerPosition)
{
	Minimap->UpdatePlayerPosition(InPlayerPosition);
}
