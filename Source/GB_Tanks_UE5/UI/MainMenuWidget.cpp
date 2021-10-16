// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ButtonSlot.h"
#include "Components/Image.h"
#include "Slate/StyleSet.h"
#include "GB_Tanks_UE5/GB_Tanks_UE5.h"

#include <Kismet/KismetSystemLibrary.h>

#include "Components/TextBlock.h"

// Applying Styles on PreConstruct to see changes in editor.
void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	//RadioButton = WidgetTree->ConstructWidget<URadioButton>();

	if (RadioBtn)
	{
		RadioBtn->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FRadioButtonsStyle>("WS_RadioButtons");
	}

	if (NewGameBtn)
	{
		NewGameBtn->SetBackgroundColor(FStyleSet::Get().GetColor("Red"));
	}

	if (OptionsBtn)
	{
		OptionsBtn->SetBackgroundColor(FStyleSet::Get().GetColor("OptionsColor"));
	}

	if (QuitBtn)
	{
		QuitBtn->SetBackgroundColor(FStyleSet::Get().GetColor("QuitGameColor"));
	}

	if (NewGameImg)
	{
		NewGameImg->SetBrush(*FStyleSet::Get().GetBrush("RedFrog"));

		if (UButtonSlot* ImgSlot = Cast<UButtonSlot>(NewGameImg->Slot))
		{
			ImgSlot->SetPadding(FStyleSet::Get().GetMargin("Margin"));
		}
	}
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NewGameBtn)
	{
		NewGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameBtnClicked);
	}

	if (OptionsBtn)
	{
		//OptionsBtn->SetContent(CreateWidget<UButton>(this));
		OptionsBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsBtnClicked);
	}

	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitBtnClicked);
	}

	if (NewGameBtnAnim)
	{
		// NewGameBtnAnim->BindToAnimationFinished(this, AnimDelegate);
		// NewGameBtnAnim->OnAnimationFinished_DEPRECATED.Add(AnimDelegate);
		AnimDelegate.BindDynamic(this, &UMainMenuWidget::OnAnimationEnded);
		NewGameBtnAnim->OnAnimationFinished_DEPRECATED.AddUnique(AnimDelegate);
	}
}

void UMainMenuWidget::OnNewGameBtnClicked()
{
	if (NewGameBtn->OnClicked.IsBound())
	{
		UE_LOG(LogTanks, Warning, TEXT("New Game Button Delegate!"));
	}

	PlayAnimationForButton(NewGameBtnAnim, NewGameBtn);
	UE_LOG(LogTanks, Warning, TEXT("New Game Button Clicked!"));

	//GameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Level_1")));
}

void UMainMenuWidget::OnOptionsBtnClicked()
{
	if (OptionsBtn->OnClicked.IsBound())
	{
		UE_LOG(LogTanks, Warning, TEXT("Options Button Delegate!"));
	}

	// Open new level
	UE_LOG(LogTanks, Warning, TEXT("Options Button Clicked!"));
}

void UMainMenuWidget::OnQuitBtnClicked()
{
	if (QuitBtn->OnClicked.IsBound())
	{
		UE_LOG(LogTanks, Warning, TEXT("Quit Button Delegate!"));
	}

	UE_LOG(LogTanks, Warning, TEXT("Quit Button Clicked!"));

	// Quit current game
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::OnAnimationEnded()
{
	UE_LOG(LogTanks, Warning, TEXT("Animation finished!"));
}

void UMainMenuWidget::PlayAnimationForButton(UWidgetAnimation* const Anim, UButton* const Btn)
{
	if (!Btn || !Anim)
	{
		return;
	}

	PlayAnimation(Anim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, true);

	// Delegates for testing
	// Anim->BindToAnimationFinished(this, AnimDelegate);

	// AnimDelegate.BindDynamic(this, &UMainMenuWidget::OnAnimationEnded);
	// Anim->OnAnimationFinished_DEPRECATED.AddUniqueDynamic(this, &UMainMenuWidget::OnAnimationEnded);
	// Anim->OnAnimationFinished_DEPRECATED.Broadcast();
}
