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
#include "MediaTexture.h"
#include "Kismet/KismetRenderingLibrary.h"

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
		ScaleSlider->SetValue(0.5f);
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

	// Check if selected media file is video file.
	bIsVideoFile = FilePath.EndsWith(".mp4", ESearchCase::IgnoreCase) ? true : false;

	if (bIsVideoFile)
	{
		InitVideoScreen();
	}
	else // If selected file isn't a video.
	{
		InitPictureScreen();
	}

	// Switching to Screen widget.
	WidgetSwitcher->SetActiveWidgetIndex(1);
	ScaleSlider->OnValueChanged.Broadcast(ScaleSlider->GetValue());
}

void UVideoPlayerWidget::InitVideoScreen() const
{
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
	/*UMediaSoundComponent* SoundComponent = NewObject<UMediaSoundComponent>(this);
	SoundComponent->SetMediaPlayer(MediaPlayer);
	SoundComponent->RegisterComponent();*/

	// Set Brush from MediaTexture.
	Screen->Brush.SetResourceObject(MediaTexture);
	const float InitialSizeX = MediaTexture->GetWidth() / 1000.f;
	const float InitialSizeY = MediaTexture->GetHeight() / 1000.f;
	Screen->SetRenderScale(FVector2D(InitialSizeX, InitialSizeY));
}

void UVideoPlayerWidget::InitPictureScreen() const
{
	if (UTexture2D* ImageTexture = UKismetRenderingLibrary::ImportFileAsTexture2D(GetWorld(), FilePath))
	{
		Screen->Brush.SetResourceObject(ImageTexture);
		const float InitialSizeX = ImageTexture->GetSizeX() / 1000.f;
		const float InitialSizeY = ImageTexture->GetSizeY() / 1000.f;
		Screen->SetRenderScale(FVector2D(InitialSizeX, InitialSizeY));
	}
}

void UVideoPlayerWidget::OnScaleChanged(float Scale)
{
	if (Screen)
	{
		const float ScalingFactor = 1000.f / Scale;
		float SizeX;
		float SizeY;

		// If playing a video file.
		if (bIsVideoFile)
		{
			SizeX = MediaTexture->GetWidth();
			SizeY = MediaTexture->GetHeight();
			Screen->SetRenderScale(FVector2D(SizeX, SizeY) / ScalingFactor);
		}
		else // If showing a picture.
		{
			if (const UTexture2D* Texture = Cast<UTexture2D>(Screen->Brush.GetResourceObject()))
			{
				SizeX = Texture->GetSizeX();
				SizeY = Texture->GetSizeY();
				Screen->SetRenderScale(FVector2D(SizeX, SizeY) / ScalingFactor);
			}
		}
	}
}
