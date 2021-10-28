// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/VideoPlayerWidget.h"

#include "DesktopPlatformModule.h"
#include "FileMediaSource.h"
#include "IDesktopPlatform.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"

void UVideoPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Setting up buttons.
	if (BrowseButton)
	{
		BrowseButton->OnClicked.AddDynamic(this, &UVideoPlayerWidget::OnBrowseButtonClicked);
	}

	if (ScaleSlider)
	{
		ScaleSlider->OnValueChanged.AddDynamic(this, &UVideoPlayerWidget::OnScaleChanged);
	}

	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UVideoPlayerWidget::OnBrowseButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked on widget!"));

	IDesktopPlatform* Platform = FDesktopPlatformModule::Get();
	if (!Platform)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get platform!"));
		return;
	}

	// Trying to open system file dialog.
	const auto ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	const FString FileTypes =
		"All Files (*.PNG, *.JPG, *.MP4)|*.PNG;*.JPG;*.MP4|"
		"Video Files (*.MP4)|*.MP4|"
		"Image Files (*.PNG, *.JPG)|*.PNG, *.JPG";
	TArray<FString> OutFileNames;
	Platform->OpenFileDialog(ParentWindowHandle, "Choose media", "", "", FileTypes, 0, OutFileNames);

	if (!OutFileNames.IsEmpty())
	{
		FilePath = OutFileNames[0];
	}

	// Initializing Media Player.
	if (!MediaPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("No Media Player!"));
		return;
	}

	UFileMediaSource* MediaSource = NewObject<UFileMediaSource>();
	MediaSource->FilePath = FilePath;
	MediaPlayer->OpenSource(MediaSource);

	// Initializing Sound Component.
	UMediaSoundComponent* SoundComponent = NewObject<UMediaSoundComponent>(this);
	SoundComponent->SetMediaPlayer(MediaPlayer);
	SoundComponent->RegisterComponent();

	// Switching to Screen widget.
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UVideoPlayerWidget::OnScaleChanged(float Scale)
{
	if (Screen)
	{
		Screen->SetRenderScale(FVector2D(Scale));
	}
}
