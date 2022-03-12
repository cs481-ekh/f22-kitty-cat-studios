// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "SMainMenuWidget.h"
#include "MenuHUD.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime\Engine\Public\ImageUtils.h>
#include "Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(500.f, 300.f);
	const FMargin ScorePadding = FMargin(550.f, 200.f);
	const FMargin HScoreContentPadding = FMargin();
	const FMargin ButtonPadding = FMargin(10.f); //This is the space between buttons

	const FText PlayDayText = LOCTEXT("PlayGameDay", "Play During the Day");
	const FText PlayNightText = LOCTEXT("PlayGameNight", "Play at Night");
	const FText PlayRainText = LOCTEXT("PlayGameRain", "Play in the Rain :)");
	const FText HighScoreText = LOCTEXT("HighScores","High Score Screen");
	const FText	QuitText = LOCTEXT("QuitGame", "Quit Game");
	const FText ReturnMain = LOCTEXT("Return","Return to Main Menu");

	const int NUMSCORES = 10; //Only need 10 scores

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

		//Need to pull the current high scores before building screen
		UE_LOG(LogTemp, Warning, TEXT("LOADING SCORES"));
		FString file = FPaths::ProjectConfigDir();
		file.Append(TEXT("highScores.txt"));
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		TArray<FString> Result;
		if (FileManager.FileExists(*file)) {
			if (FFileHelper::LoadFileToStringArray(Result, *file, FFileHelper::EHashOptions::None)) {
				UE_LOG(LogTemp, Warning, TEXT("Score Array Loaded"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not Load Scores"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
		}
		FText score0 = FText::FromString(Result[0]);
		FText score1 = FText::FromString(Result[1]);
		FText score2 = FText::FromString(Result[2]);
		FText score3 = FText::FromString(Result[3]);
		FText score4 = FText::FromString(Result[4]);
		FText score5 = FText::FromString(Result[5]);
		FText score6 = FText::FromString(Result[6]);
		FText score7 = FText::FromString(Result[7]);
		FText score8 = FText::FromString(Result[8]);
		FText score9 = FText::FromString(Result[9]);

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
				.Padding(ScorePadding)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score0) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score1) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot() 
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score2) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score3) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score4) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score5) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score6) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score7) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score8) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]
					+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(score9) //these need to be FText not FString
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Orange)
					]

			]
			+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(HScoreContentPadding)
				[

				//Title Text
				SNew(SVerticalBox)

				// Main Menu Button
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

				//Quit Game Button
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