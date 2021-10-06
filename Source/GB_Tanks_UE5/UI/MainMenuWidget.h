// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"

#include "MainMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** New Game Button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* NewGameBtn;

	/** Options Button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* OptionsBtn;

	/** Quit Game Button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitBtn;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* NewGameBtnAnim;

protected:
	FWidgetAnimationDynamicEvent AnimDelegate;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnNewGameBtnClicked();

	UFUNCTION()
	virtual void OnOptionsBtnClicked();

	UFUNCTION()
	virtual void OnQuitBtnClicked();

	UFUNCTION()
	virtual void OnAnimationEnded();

protected:
	void PlayAnimationForButton(UWidgetAnimation* const Anim, UButton* const Btn);
};
