// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

UENUM(BlueprintType)
enum class ERadioButton : uint8
{
	RB_0,
	RB_1,
	RB_2
};

DECLARE_DELEGATE_OneParam(FOnButtonStateChanged, ERadioButton /* ButtonId */)

/**
 * 
 */
class GB_TANKS_UE5_API SRadioButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadioButton)
		{
		}

		SLATE_EVENT(FOnButtonStateChanged, OnButtonStateChanged)

	SLATE_END_ARGS()

protected:
	ERadioButton CurrentButton = ERadioButton::RB_0;
	FOnButtonStateChanged OnButtonStateChangedEvent;

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedRef<SCheckBox> CreateRadioButton(ERadioButton ButtonId, FString Text);
	ECheckBoxState IsButtonChecked(ERadioButton ButtonId) const;
	void OnButtonStateChanged(ECheckBoxState NewState, ERadioButton ButtonId);
};
