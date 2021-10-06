// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Slate/SMiniMapWidget.h"

#include "MiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API UMiniMapWidget : public UWidget
{
	GENERATED_BODY()

public:
	/** Texture for the Minimap */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	UTexture2D* MinimapTexture;

	/** Texture for the PlayerIcon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	UTexture2D* PlayerIconTexture;

	/** Image for the Minimap */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush MinimapBrush;

	/** Image for the PlayerIcon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush PlayerIconBrush;

	/** A bindable delegate for the MinimapBrush. */
	UPROPERTY()
	FGetSlateBrush MinimapBrushDelegate;

	/** A bindable delegate for the PlayerIconBrush. */
	UPROPERTY()
	FGetSlateBrush PlayerIconBrushDelegate;

	/** Color and opacity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( sRGB="true"))
	FLinearColor ColorAndOpacity;

	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor ColorAndOpacityDelegate;

protected:
	/** Controlled SWidget */
	TSharedPtr<SMiniMapWidget> MiniMap;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;

	const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;

public:
	void UpdatePlayerPosition(const FVector2D& InPosition);

protected:
	/** Property bindings for SynchronizeProperties() */
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
};
