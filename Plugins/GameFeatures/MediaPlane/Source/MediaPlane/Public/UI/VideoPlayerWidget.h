// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "VideoPlayerWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class UImage;
class USlider;
class UMediaPlayer;

/**
 * 
 */
UCLASS()
class MEDIAPLANE_API UVideoPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/** Main frame Widget Switcher */
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	/** Browse Button */
	UPROPERTY(meta=(BindWidget))
	UButton* BrowseButton;

	/** Image widget used to render a picture from Media Player */
	UPROPERTY(meta=(BindWidget))
	UImage* Screen;

	/** Slider to adjust Screen scale */
	UPROPERTY(meta=(BindWidget))
	USlider* ScaleSlider;

public:
	/** Media Player requires MediaAssets dependency in build properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Media Player")
	UMediaPlayer* MediaPlayer;

	/** Path for selected media file */
	UPROPERTY()
	FString FilePath;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBrowseButtonClicked();

	UFUNCTION()
	void OnScaleChanged(float Scale);
};
