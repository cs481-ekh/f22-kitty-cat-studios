// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
//#include "CoreMinimal.h"

/**
 * 
 */
//BRONCODROME_API -- add after "class" if not working properly
class SMainMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)
	SLATE_ARGUMENT(TWeakObjectPtr<class UTexture2D>, broncyImage)

	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	FReply OnPlayDayClicked() const;
	FReply OnPlayNightClicked() const;
	FReply OnPlayRainClicked() const;
	FReply OnQuitClicked() const;
	FReply OnHScoreClicked() const;
	FReply OnReturnToMainClicked() const;
	 
	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	TWeakObjectPtr<class UTexture2D> broncyImage;

	virtual bool SupportsKeyboardFocus() const override { return true; };


};
