// Fill out your copyright notice in the Description page of Project Settings.

#include "RadioButton.h"

void URadioButton::ReleaseSlateResources(bool bReleaseChildren)
{
	RadioButton.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> URadioButton::RebuildWidget()
{
	RadioButton = SNew(SRadioButton)
		.Style(&WidgetStyle)
		.OnButtonStateChanged_UObject(this, &URadioButton::OnButtonStateChanged);

	return RadioButton.ToSharedRef();
}

void URadioButton::OnButtonStateChanged(ERadioButton ButtonId)
{
	OnButtonStateChangedEvent.Broadcast(ButtonId);
}
