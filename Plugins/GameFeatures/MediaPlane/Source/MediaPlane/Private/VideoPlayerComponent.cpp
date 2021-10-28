// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoPlayerComponent.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "MediaSoundComponent.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/FileMediaSource.h"

// Sets default values for this component's properties
UVideoPlayerComponent::UVideoPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UVideoPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!MediaPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("No Media Player!"));
		return;
	}

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
	FString Path;
	TArray<FString> OutFileNames;
	Platform->OpenFileDialog(ParentWindowHandle, "Choose media", "", "", FileTypes, 0, OutFileNames);

	if (!OutFileNames.IsEmpty())
	{
		Path = OutFileNames[0];
	}

	UFileMediaSource* MediaSource = NewObject<UFileMediaSource>();
	MediaSource->FilePath = Path;
	MediaPlayer->OpenSource(MediaSource);

	UMediaSoundComponent* SoundComponent = NewObject<UMediaSoundComponent>();
	SoundComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SoundComponent->SetMediaPlayer(MediaPlayer);
	SoundComponent->RegisterComponent();
}
