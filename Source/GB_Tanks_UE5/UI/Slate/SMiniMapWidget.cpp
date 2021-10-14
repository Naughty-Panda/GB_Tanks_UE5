// Fill out your copyright notice in the Description page of Project Settings.

#include "SMiniMapWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMiniMapWidget::Construct(const FArguments& InArgs)
{
	/** Brushes */
	MinimapImg = FInvalidatableBrushAttribute(InArgs._MinimapImg);
	PlayerIconImg = FInvalidatableBrushAttribute(InArgs._PlayerImg);

	/** Textures */
	MinimapTexture = InArgs._MinimapTexture;
	PlayerIconTexture = InArgs._PlayerIconTexture;

	/** Color and Opacity */
	ColorAndOpacity = InArgs._ColorAndOpacity;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SMiniMapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                              FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Setup user-defined Brushes.
	const FSlateBrush* MinimapImgBrush = MinimapImg.GetImage().Get();
	const FSlateBrush* PlayerImgBrush = PlayerIconImg.GetImage().Get();

	// Setup Textures for default Brush.
	FSlateBrush MinimapBrush;
	MinimapBrush.SetResourceObject(MinimapTexture);
	const FVector2D DefaultMinimapSize(256.f);
	const float MinimapScale = (AllottedGeometry.GetLocalSize() / DefaultMinimapSize).GetMin();
	const FVector2D ScaledMinimapSize = DefaultMinimapSize * MinimapScale;

	// TODO: Separate Texture and Brush paint methods.
	// Painting Minimap Image first.
	if ((MinimapImgBrush != nullptr) && (MinimapImgBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
	{
		const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
		const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
		const FLinearColor FinalColorAndOpacity(/* InWidgetStyle.GetColorAndOpacityTint() * ColorAndOpacity.Get().GetColor(InWidgetStyle) * */ MinimapImgBrush->GetTint(InWidgetStyle));
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(ScaledMinimapSize, FSlateLayoutTransform()),
			&MinimapBrush,
			DrawEffects,
			FinalColorAndOpacity
			);
	}

	++LayerId;

	// Painting Player icon on Minimap.
	if ((PlayerImgBrush != nullptr) && (PlayerImgBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
	{
		FSlateBrush PlayerIconBrush;
		PlayerIconBrush.SetResourceObject(PlayerIconTexture);

		constexpr float PlayerIconScale = 1.f;
		const FVector2D PlayerIconSize(50.f);
		const FVector2D PlayerIconCenter(PlayerIconSize * 0.5f);
		const FVector2D PlayerIconTranslation(ScaledMinimapSize * 0.5f - PlayerIconCenter + PlayerPosition);
		const FSlateLayoutTransform PlayerIconTransform(PlayerIconScale, PlayerIconTranslation);
		
		const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
		const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
		const FLinearColor FinalColorAndOpacity(/* InWidgetStyle.GetColorAndOpacityTint() * ColorAndOpacity.Get().GetColor(InWidgetStyle) * */ PlayerImgBrush->GetTint(InWidgetStyle));

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(PlayerIconSize, PlayerIconTransform),
			&PlayerIconBrush,
			DrawEffects,
			FinalColorAndOpacity
			);
	}

	return LayerId;
}

void SMiniMapWidget::SetBrushes(TAttribute<const FSlateBrush*> InMinimapImage, TAttribute<const FSlateBrush*> InPlayerIconImage)
{
	MinimapImg.SetImage(*this, InMinimapImage);
	PlayerIconImg.SetImage(*this, InPlayerIconImage);
}

void SMiniMapWidget::SetTextures(const UTexture2D* MinimapTex, const UTexture2D* PlayerIconTex)
{
	MinimapTexture = const_cast<UTexture2D*>(MinimapTex);
	PlayerIconTexture = const_cast<UTexture2D*>(PlayerIconTex);
}

void SMiniMapWidget::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	SetAttribute(ColorAndOpacity, InColorAndOpacity, EInvalidateWidgetReason::Paint);
}

void SMiniMapWidget::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	SetColorAndOpacity(TAttribute<FSlateColor>(InColorAndOpacity));
}

void SMiniMapWidget::UpdatePlayerPosition(const FVector2D& InPosition)
{
	/* Minimap Texture size = 256 * 256
	 * World Map size = 12800 * 12800
	 */
	constexpr float WorldToTextureRatio = 256.f / 12800.f;

	// Swap Player's X and Y coordinates and invert X Pos to match texture's coordinates.
	const FVector2D AdjustedPosition(InPosition.Y, -InPosition.X);
	PlayerPosition = AdjustedPosition * WorldToTextureRatio;
}
