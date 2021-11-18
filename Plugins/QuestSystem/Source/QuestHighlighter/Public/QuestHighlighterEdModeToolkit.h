// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FQuestHighlighterEdModeToolkit : public FModeToolkit
{
public:

	FQuestHighlighterEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:
	FLinearColor GetNPCColor() const;
	void SetNPCColor(FLinearColor NewColor);
	FLinearColor GetObjectivesColor() const;
	void SetObjectivesColor(FLinearColor NewColor);
	FReply OnOpenNPCColor(const FGeometry&, const FPointerEvent&);
	FReply OnOpenObjectivesColor(const FGeometry&, const FPointerEvent&);
	
private:
	TSharedPtr<SWidget> ToolkitWidget;
};
