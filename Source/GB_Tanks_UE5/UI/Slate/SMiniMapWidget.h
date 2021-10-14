// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class GB_TANKS_UE5_API SMiniMapWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMiniMapWidget)
	: _MinimapImg(FCoreStyle::Get().GetDefaultBrush())
	, _PlayerImg(FCoreStyle::Get().GetDefaultBrush())
	, _MinimapTexture()
	, _PlayerIconTexture()
	, _ColorAndOpacity(FLinearColor::White)
	{
	}

	/** Brush used to draw the Minimap. */
	SLATE_ATTRIBUTE(const FSlateBrush*, MinimapImg)

	/** Brush used to draw the Player icon. */
	SLATE_ATTRIBUTE(const FSlateBrush*, PlayerImg)

	/** Minimap texture. */
	SLATE_ARGUMENT(UTexture2D*, MinimapTexture)

	/** Player icon texture. */
	SLATE_ARGUMENT(UTexture2D*, PlayerIconTexture)

	/** Color and opacity */
	SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

	SLATE_END_ARGS()

protected:
	/** The slate brushes to draw for the image that we can invalidate. */
	FInvalidatableBrushAttribute MinimapImg;
	FInvalidatableBrushAttribute PlayerIconImg;

	/** Textures for Minimap and PlayerIcon. */
	UTexture2D* MinimapTexture = nullptr;
	UTexture2D* PlayerIconTexture = nullptr;

	/** Color and opacity scale for this image */
	TAttribute<FSlateColor> ColorAndOpacity;

	/** Player position */
	FVector2D PlayerPosition;

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
	                      int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
	void SetBrushes(TAttribute<const FSlateBrush*> InMinimapImage, TAttribute<const FSlateBrush*> InPlayerIconImage);
	void SetTextures(const UTexture2D* MinimapTex, const UTexture2D* PlayerIconTex);
	void SetColorAndOpacity( const TAttribute<FSlateColor>& InColorAndOpacity );
	void SetColorAndOpacity( FLinearColor InColorAndOpacity );
	void UpdatePlayerPosition(const FVector2D& InPosition);
};
