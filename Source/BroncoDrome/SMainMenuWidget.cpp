// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "SMainMenuWidget.h"
#include "MenuHUD.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime\Engine\Public\ImageUtils.h>

#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(500.f, 300.f); 
	const FMargin HScoreContentPadding = FMargin();
	const FMargin ButtonPadding = FMargin(10.f); //This is the space between buttons

	const FText PlayDayText = LOCTEXT("PlayGameDay", "Play During the Day");
	const FText PlayNightText = LOCTEXT("PlayGameNight", "Play at Night");
	const FText PlayRainText = LOCTEXT("PlayGameRain", "Play in the Rain :)");
	const FText HighScoreText = LOCTEXT("HighScores","High Score Screen");
	const FText	QuitText = LOCTEXT("QuitGame", "Quit Game");
	const FText ReturnMain = LOCTEXT("Return","Return to Main Menu");

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 25.f;

	// setup for background image
	broncyImage = InArgs._broncyImage; //Don't be decieved! this can be either the main menu or high score image
	const FSlateDynamicImageBrush* BroncyImage;
	BroncyImage = new FSlateDynamicImageBrush(broncyImage.Get(), FVector2D(942, 614), FName("BroncyImage"));
	// above sets broncyImage to brush, and set size of image

	//IF mainOrHScore is 0
	if (!OwningHUD->mainOrHScore) {
		//THEN build the main menu
		ChildSlot[
			SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(BroncyImage)
				]

			+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				.Padding(ContentPadding)
				[

					//Title Text
					SNew(SVerticalBox)

					// Play during day text
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnPlayDayClicked)
				.ButtonColorAndOpacity(FColor::Blue)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(PlayDayText)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::Orange)
				]

				]

			// Play during night
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnPlayNightClicked)
				.ButtonColorAndOpacity(FColor::Blue)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(PlayNightText)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::Orange)
				]

				]

			// Play rainy map
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnPlayRainClicked)
				.ButtonColorAndOpacity(FColor::Blue)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(PlayRainText)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::Orange)
				]

				]
			//High Score Button
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)

				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnHScoreClicked)
				.ButtonColorAndOpacity(FColor::Blue)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(HighScoreText)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::Orange)
				]
				]

			//Quit Game Button Text
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)

				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnQuitClicked)
				.ButtonColorAndOpacity(FColor::Blue)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(QuitText)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::Orange)
				]

				]


				]
		];
	}
	//ELSE IF mainOrHScore is 1
	else if (OwningHUD->mainOrHScore) { //I'm being overly explicit here just in case anyone adds more screens later
		//THEN build the high score screen
		ChildSlot[
			SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(BroncyImage)
				]

			+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(HScoreContentPadding)
				[

					//Title Text
					SNew(SVerticalBox)

					// Play during day text
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnReturnToMainClicked)
				.ButtonColorAndOpacity(FColor::Blue)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(ReturnMain)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::Orange)
				]

				]

			//Quit Game Button Text
			+ SVerticalBox::Slot()
				.Padding(ButtonPadding)

				[
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnQuitClicked)
				.ButtonColorAndOpacity(FColor::Blue)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(QuitText)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::Orange)
				]

				]


				]
		];
	}

}

FReply SMainMenuWidget::OnPlayDayClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->RemoveMenu();
	}

	UGameplayStatics::OpenLevel(GWorld, "Broncodrome_Day");

	return FReply::Handled();
}

FReply SMainMenuWidget::OnPlayNightClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->RemoveMenu();
	}

	UGameplayStatics::OpenLevel(GWorld, "Broncodrome_Night");

	return FReply::Handled();
}

FReply SMainMenuWidget::OnPlayRainClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->RemoveMenu();
	}

	UGameplayStatics::OpenLevel(GWorld, "Broncodrome_Rain");

	return FReply::Handled();
}

FReply SMainMenuWidget::OnQuitClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			PC->ConsoleCommand("quit");
		}
	}

	return FReply::Handled();
}

//This is the handler for the high score button on the main menu
FReply SMainMenuWidget::OnHScoreClicked() const
{
	if (OwningHUD.IsValid()) 
	{
		OwningHUD->RemoveMenu(); //get rid of current screen
		OwningHUD->ShowMenu(1);  //show the high score screen
	}
	return FReply::Handled();
}

FReply SMainMenuWidget::OnReturnToMainClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->RemoveMenu();
		OwningHUD->ShowMenu(0);
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE