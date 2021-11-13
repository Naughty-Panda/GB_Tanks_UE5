// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GB_Tanks_UE5/GameStructs.h"

#include "UIManager.generated.h"

class UQuestLogWidget;


/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API AUIManager : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetTypes;

private:
	UPROPERTY()
	UUserWidget* ActiveWidget = nullptr;

	EWidgetType ActiveWidgetType = EWidgetType::WT_None;

	/**
	 * Quest System
	 */

private:
	UPROPERTY()
	UQuestLogWidget* QuestLog;

	UPROPERTY(EditAnywhere, Category="Widgets|Quest")
	TSubclassOf<UQuestLogWidget> QuestLogWidgetClass;

public:
	AUIManager();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="UI")
	void SetActiveWidget(const EWidgetType WidgetType, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category="Quest")
	void ToggleQuestLogVisibility();

	void AddWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder = 0);
	void RemoveActiveWidget();
	void UpdatePlayerPositionOnMinimap(const FVector2D& InPosition);
};
