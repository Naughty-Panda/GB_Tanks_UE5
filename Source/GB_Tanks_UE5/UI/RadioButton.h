// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Slate/SRadioButton.h"

#include "RadioButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonStateChangedEvent, ERadioButton, ButtonId);

/**
 * 
 */
UCLASS()
class GB_TANKS_UE5_API URadioButton : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SRadioButton> RadioButton;

	UPROPERTY(BlueprintAssignable, Category="UI Events")
	FOnButtonStateChangedEvent OnButtonStateChangedEvent;

public:
	UPROPERTY(EditAnywhere, Category=Appearance)
	FRadioButtonsStyle WidgetStyle;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void OnButtonStateChanged(ERadioButton ButtonId);
};
