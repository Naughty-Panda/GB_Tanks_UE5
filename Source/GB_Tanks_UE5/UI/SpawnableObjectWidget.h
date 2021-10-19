// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SpawnableObjectWidget.generated.h"

class UTextBlock;
class ATankPlayerController;

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API USpawnableObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnableClass;

	UPROPERTY(EditAnywhere)
	FText SpawnableObjectName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpawnableObjectNameTextBlock;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> TooltipClass;

protected:
	UPROPERTY()
	AActor* SpawnedActor;

	UPROPERTY()
	ATankPlayerController* PlayerController;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void OnMouseButtonUp();
};
