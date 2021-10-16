#include "StyleSet.h"
#include "Engine.h"
#include "Slate/SlateGameResources.h"
#include "Styling/ISlateStyle.h"
//#include "Styling/SlateStyleMacros.h"

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

TSharedPtr<ISlateStyle> FindStyle(const FString& ScopeToDirectory)
{
	TSharedPtr<FSlateGameResources> Style = FSlateGameResources::New(FName("StyleSet"), ScopeToDirectory, ScopeToDirectory /* BasePath */);

	// Adding properties to the Style Set.

	Style->Set("NewGameColor", FLinearColor::Yellow);
	Style->Set("OptionsColor", FLinearColor::Blue);
	Style->Set("QuitGameColor", FLinearColor::Red);
	Style->Set("Margin", FMargin(10.f, 10.f));

	// Error reading file!
	// Style->Set("RedFrog", new IMAGE_BRUSH("RedFrog", FVector2D(256.f, 256.f)));

	// RadioButtons CheckBox style.
	// const FCheckBoxStyle RadioButtonStyleCopy = FCoreStyle::Get().GetWidgetStyle<FCheckBoxStyle>("RadioButton");
	// Style->Set("RadioButton", RadioButtonStyleCopy);

	return Style;
}

#undef IMAGE_BRUSH

TSharedPtr<ISlateStyle> FStyleSet::StylePtr = nullptr;

void FStyleSet::Initialize()
{
	Shutdown();

	// Path /Game/ == /Content/ on disk.
	StylePtr = FindStyle("/Game/UI/Style");

	// Absolute path for RedFrog Brush.
	// StylePtr = FindStyle(FPaths::ProjectContentDir() / "/UI/Style");

	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
}

void FStyleSet::Shutdown()
{
	if (StylePtr.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		ensure(StylePtr.IsUnique());
		StylePtr.Reset();
	}
}

const ISlateStyle& FStyleSet::Get()
{
	if (!StylePtr.IsValid())
	{
		Initialize();
	}
	return *StylePtr;
}
