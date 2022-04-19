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

	void InitBroncoSave(int level) const; //helper method to initialize the save
	FReply OnPlayClicked() const;
	FReply OnPlayDayClicked() const;
	FReply OnPlayNightClicked() const;
	FReply OnPlayRainClicked() const;
	FReply OnQuitClicked() const;
	FReply OnHScoreClicked() const;
	void OnHScoreDebug(const FText& InText, ETextCommit::Type);
	FReply OnReturnToMainClicked() const;
	void BuildMenu(int hOrM);
	FReply OnTutClicked() const;
	FReply OnNextTutClicked() const;
	 
	TArray<FString> Result;
	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	TWeakObjectPtr<class UTexture2D> broncyImage;

	FText score0, score1, score2, score3, score4, score5, score6, score7, score8, score9;

	virtual bool SupportsKeyboardFocus() const override { return true; };


};
