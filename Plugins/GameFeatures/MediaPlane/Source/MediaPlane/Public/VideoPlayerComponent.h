// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VideoPlayerComponent.generated.h"

class UMediaPlayer;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MEDIAPLANE_API UVideoPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Media Player requires MediaAssets dependency in build properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Media Player")
	UMediaPlayer* MediaPlayer;

public:
	// Sets default values for this component's properties
	UVideoPlayerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
