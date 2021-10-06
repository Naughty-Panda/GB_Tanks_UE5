// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMapWidget.h"
#include "Components/Image.h"
#include "Slate/SMiniMapWidget.h"

void UMiniMapWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MiniMap.Reset();
}

TSharedRef<SWidget> UMiniMapWidget::RebuildWidget()
{
	MiniMap = SNew(SMiniMapWidget);
	return MiniMap.ToSharedRef();
}

void UMiniMapWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FSlateColor> ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
	TAttribute<const FSlateBrush*> MinimapImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, MinimapBrush, const FSlateBrush*, ConvertImage);
	TAttribute<const FSlateBrush*> PlayerIconImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, PlayerIconBrush, const FSlateBrush*, ConvertImage);

	if (MiniMap.IsValid())
	{
		// Set brush in Slate Minimap class.
		MiniMap->SetBrushes(MinimapImageBinding, PlayerIconImageBinding);
		MiniMap->SetColorAndOpacity(ColorAndOpacityBinding);

		// Set textures in Slate Minimap class.
		MiniMap->SetTextures(MinimapTexture, PlayerIconTexture);
	}
}

const FSlateBrush* UMiniMapWidget::ConvertImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UMiniMapWidget* MutableThis = const_cast<UMiniMapWidget*>(this);
	MutableThis->MinimapBrush = InImageAsset.Get();

	return &MinimapBrush;
}

void UMiniMapWidget::UpdatePlayerPosition(const FVector2D& InPosition)
{
	if (MiniMap.IsValid())
	{
		MiniMap->UpdatePlayerPosition(InPosition);
	}
}
