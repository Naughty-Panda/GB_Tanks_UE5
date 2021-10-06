// Fill out your copyright notice in the Description page of Project Settings.

#include "SRadioButton.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadioButton::Construct(const FArguments& InArgs)
{
	OnButtonStateChangedEvent = InArgs._OnButtonStateChanged;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButton::RB_0, "Button 0")
		]
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButton::RB_1, "Button 1")
		]
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButton::RB_2, "Button 2")
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SCheckBox> SRadioButton::CreateRadioButton(ERadioButton ButtonId, FString Text)
{
	return
		SNew(SCheckBox)
			.IsChecked(this, &SRadioButton::IsButtonChecked, ButtonId)
			.OnCheckStateChanged(this, &SRadioButton::OnButtonStateChanged, ButtonId)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Text))
		];
}

ECheckBoxState SRadioButton::IsButtonChecked(ERadioButton ButtonId) const
{
	return CurrentButton == ButtonId ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SRadioButton::OnButtonStateChanged(ECheckBoxState NewState, ERadioButton ButtonId)
{
	if (NewState == ECheckBoxState::Checked)
	{
		CurrentButton = ButtonId;
		OnButtonStateChangedEvent.ExecuteIfBound(CurrentButton);
	}
}
