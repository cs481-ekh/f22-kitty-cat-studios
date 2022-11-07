// // Copyright (C) Dromies 2021. All Rights Reserved.

#include "SMainMenuWidget.h"
#include "BroncoSaveGame.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MenuHUD.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
//#include <Runtime\Engine\Public\ImageUtils.h>

#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments &InArgs) {
  bCanSupportFocus = true;

  OwningHUD = InArgs._OwningHUD;

  // setup for background image
  broncyImage = InArgs._broncyImage;  // Don't be decieved! this can be either
                                      // the main menu or high score image
  sdpLogo = InArgs._sdpLogo;

  // Need to pull the current high scores before building screen
  FString file = FPaths::ProjectConfigDir();
  file.Append(TEXT("highScores.txt"));
  IPlatformFile &FileManager = FPlatformFileManager::Get().GetPlatformFile();
  if (FileManager.FileExists(*file)) {
    // File loaded successfully
  } else {
    // Try to create the required highscores file
    UE_LOG(LogTemp, Warning,
           TEXT("FileManipulation: INFO: Initializing new Score file"));

    FString FilePath =
        FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir()) +
        TEXT("/highScores.txt");
    FFileHelper::SaveStringToFile(
        TEXT("BEN 3000\nMAR 2500\nDJR 2000\nDOU 1500\nROS 1000\nJOY 800\nSAU 600\nPLC 400\nTST 200\nDBG 0"), *FilePath, FFileHelper::EEncodingOptions::AutoDetect,
        &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
  }

  if (FileManager.FileExists(*file)) {
    if (FFileHelper::LoadFileToStringArray(Result, *file,
                                           FFileHelper::EHashOptions::None)) {
      UE_LOG(LogTemp, Warning, TEXT("Score Array Loaded"));
    } else {
      UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not Load Scores"));
    }
  } else {
    UE_LOG(LogTemp, Warning,
           TEXT("FileManipulation: ERROR: Can not read the file because it was "
                "not found."));
    UE_LOG(LogTemp, Warning,
           TEXT("FileManipulation: Expected file location: %s"), *file);
  }

  BuildMenu(OwningHUD->mainOrHScore);
}


void SMainMenuWidget::InitBroncoSave(int level, FName difficulty, bool practiceMode, FName runnerSelected) const {
  // Create and save the initial values. Nothing else is needed here "save" will
  // have the name "curr" for it's save slot.
  if (UBroncoSaveGame *save =
          Cast<UBroncoSaveGame>(UGameplayStatics::CreateSaveGameObject(
              UBroncoSaveGame::StaticClass()))) {
    save->score = 0;
    save->mapsBeaten = level;
    save->runnerSelection = runnerSelected;
    save->practiceMode = practiceMode;
    save->difficultySetting = difficulty;
    UGameplayStatics::SaveGameToSlot(save, save->SaveName, 0);
  }
}


FReply SMainMenuWidget::OnPlayClicked(FName difficulty, FName runnerSelected) const {
  if (OwningHUD.IsValid()) {
    InitBroncoSave(-1, difficulty, false, runnerSelected);  // 0 will make it so that 3 levels will be played
    OwningHUD->RemoveMenu();
  }

  UGameplayStatics::OpenLevel(GWorld, "Broncodrome_Day");

  return FReply::Handled();
}

FReply SMainMenuWidget::OnFreePlayClicked(FName difficulty, FName runnerSelected) const {
  if (OwningHUD.IsValid()) {
    InitBroncoSave(3, difficulty, true, runnerSelected);  // 3 will make it so that only 1 level will be played
    OwningHUD->RemoveMenu();
  }

  UGameplayStatics::OpenLevel(GWorld, "LevelSelectLevel");

  return FReply::Handled();
}

FReply SMainMenuWidget::OnQuitClicked() const {
  if (OwningHUD.IsValid()) {
    if (APlayerController *PC = OwningHUD->PlayerOwner) {
      PC->ConsoleCommand("quit");
    }
  }

  return FReply::Handled();
}
void SMainMenuWidget::OnHScoreDebug(const FText &InText, ETextCommit::Type) {
  FString toPrint = InText.ToString();
  FString pHolder = InText.ToString();
  FString playerScore = toPrint.RightChop(
      4);  // Will remove the player name so we can get the score
  toPrint.LeftInline(
      3,
      true);  // Makes the given string the given size, true makes it shrinkable
  FString trimmed = toPrint.TrimEnd();
  int scoreInt =
      FCString::Atoi(*playerScore);  // Atoi converts string to numeric (in this
                                     // case int) "Ascii To Integer" Atoi

  int nameNumber = 0;
  for (int i = 0; i < 10; i++) {
    if (trimmed.Contains(FString::FromInt(i))) {
      nameNumber = 1;
      break;
    }
  }

  // IF the playerScore has non-numbers in the score area
  // This is almost certainly casued by have a name that is too long
  if (!playerScore.IsNumeric()) {
    // THEN print an error
    UE_LOG(LogTemp, Warning, TEXT("NON-NUMERIC DIGITS FOUND IN SCORE"));
  // ELSE IF the playerScore is too short
  } else if (trimmed.Len() < 3 || trimmed.IsEmpty()) {
    // THEN print an error
    UE_LOG(LogTemp, Warning, TEXT("NAME NOT LONG ENOUGH"));
  // ELSE IF name has a number in it
  } else if (nameNumber) {
    // THEN print an error
    UE_LOG(LogTemp, Warning, TEXT("NAME CONTAINS NUMBERS"));
  } else {
    // NOTE: Result[] is an array of FStrings that contains the current scores

    // Check if the player score should even be on the board
    FString checkScore = Result[9].RightChop(4);
    int checkInt = FCString::Atoi(*checkScore);
    // IF the player score is at least larger than the lowest score
    if (checkInt < scoreInt) {
      // Get the current scores and compare them with the player score
      int placed = 0;
      for (int i = 8; i >= 0; i--) {
        FString currScore = Result[i].RightChop(4);
        int currInt = FCString::Atoi(*currScore);
        // IF player score is less than the current score
        if (currInt > scoreInt) {
          // THEN we need to place the player score into the previous slot
          // NOTE: Since we've already checked if the players score should be on
          //      the board we can assume this will work
          Result[i + 1] = pHolder;
          placed = 1;
          break;
        }
        // ELSE shift the scores down
        else
          Result[i + 1] = Result[i];
      }
      // IF the score is larger than all other scores
      if (!placed) {
        // THEN put it in the highest score
        Result[0] = pHolder;
      }
      // UPDATE THE SCORE FILE
      FString file = FPaths::ProjectConfigDir();
      file.Append(TEXT("highScores.txt"));
      IPlatformFile &FileManager =
          FPlatformFileManager::Get().GetPlatformFile();
      if (FileManager.FileExists(*file)) {
        if (FFileHelper::SaveStringArrayToFile(Result, *file)) {
          UE_LOG(LogTemp, Warning, TEXT("Score Array Updated"));
        } else {
          UE_LOG(LogTemp, Warning,
                 TEXT("FileManipulation: Did not update Scores"));
        }
      } else {
        UE_LOG(LogTemp, Warning,
               TEXT("FileManipulation: ERROR: Can not read the file because it "
                    "was not found."));
        UE_LOG(LogTemp, Warning,
               TEXT("FileManipulation: Expected file location: %s"), *file);
      }
      // Rebuild the high score screen
      BuildMenu(1);
    }
  }
}

// This is the handler for the difficulty selection button on the main menu
FReply SMainMenuWidget::OnDifficultyClicked() const {
  if (OwningHUD.IsValid()) {
    OwningHUD->RemoveMenu();  // get rid of current screen
    OwningHUD->ShowMenu(5);   // show the difficulty selection screen
  }
  return FReply::Handled();
}

FReply SMainMenuWidget::onRunnerSelectionClicked() const { 
    if (OwningHUD.IsValid()) {
      OwningHUD->RemoveMenu();
      OwningHUD->ShowMenu(6);
    }
    return FReply::Handled();
}

// This is the handler for the high score button on the main menu
FReply SMainMenuWidget::OnHScoreClicked() const {
  if (OwningHUD.IsValid()) {
    OwningHUD->RemoveMenu();  // get rid of current screen
    OwningHUD->ShowMenu(1);   // show the high score screen
  }
  return FReply::Handled();
}

FReply SMainMenuWidget::OnReturnToMainClicked() const {
  if (OwningHUD.IsValid()) {
    OwningHUD->RemoveMenu();
    OwningHUD->ShowMenu(0);
  }
  return FReply::Handled();
}

// This is the handler for the tutorial button on the main menu
FReply SMainMenuWidget::OnTutClicked() const {
  if (OwningHUD.IsValid()) {
    OwningHUD->RemoveMenu();  // get rid of current screen
    OwningHUD->ShowMenu(2);   // show the first tutorial screen
  }
  return FReply::Handled();
}

// This is the handler for the next tutorial button
FReply SMainMenuWidget::OnNextTutClicked() const {
  if (OwningHUD.IsValid()) {
    OwningHUD->RemoveMenu();  // get rid of current screen
    OwningHUD->ShowMenu(3);   // show the next tutorial screen
  }
  return FReply::Handled();
}

// This is the handler for the sdp logo button
FReply SMainMenuWidget::OnCredClicked() const {
  if (OwningHUD.IsValid()) {
    OwningHUD->RemoveMenu();  // get rid of current screen
    OwningHUD->ShowMenu(4);   // show the credit screen
  }
  return FReply::Handled();
}

void SMainMenuWidget::BuildMenu(int hOrM) {
  const FMargin ContentPadding = FMargin(100.f, 300.f);
  const FMargin LogoPadding = FMargin(100.f, 75.f);
  const FMargin ScorePadding = FMargin(550.f, 200.f);
  const FMargin TutPadding = FMargin(100.f, 75.f);
  const FMargin HScoreContentPadding = FMargin();
  const FMargin DifficultyPadding = FMargin(550.f, 100.f);
  const FMargin SelectionPadding = FMargin(550.f, 150.f);
  const FMargin ButtonPadding =
      FMargin(10.f);  // This is the space between buttons

  const FText PlayText = LOCTEXT("PlayGame", "Play");
  const FText FreePlayText = LOCTEXT("FreePlayGame", "Free Play");
  const FText SelectDifficultyText = LOCTEXT("SelectDifficulty", "Select Difficulty");
  const FText SelectRunnerText = LOCTEXT("SelectRunner", "Select Runner");
  const FText HighScoreText = LOCTEXT("HighScores", "High Scores");
  const FText QuitText = LOCTEXT("QuitGame", "Quit Game");
  const FText ReturnMain = LOCTEXT("Return", "Return to Main Menu");
  const FText tutText = LOCTEXT("Tutorial", "Tutorial");
  const FText NextText = LOCTEXT("next", "Next");
  const FText BackText = LOCTEXT("back", "Back");

  const int NUMSCORES = 10;  // Only need 10 scores

  FSlateFontInfo ButtonTextStyle =
      FCoreStyle::Get().GetFontStyle("EmbossedText");
  ButtonTextStyle.Size = 25.f;

  const FSlateDynamicImageBrush *BroncyImage;
  BroncyImage = new FSlateDynamicImageBrush(
      broncyImage.Get(), FVector2D(942, 614), FName("BroncyImage"));
  const FSlateDynamicImageBrush *SDPLogo;
  SDPLogo = new FSlateDynamicImageBrush(sdpLogo.Get(), FVector2D(300, 150),
                                        FName("SDPLogo"));
  // above sets broncyImage to brush, and set size of image

  score0 = FText::FromString(Result[0]);
  score1 = FText::FromString(Result[1]);
  score2 = FText::FromString(Result[2]);
  score3 = FText::FromString(Result[3]);
  score4 = FText::FromString(Result[4]);
  score5 = FText::FromString(Result[5]);
  score6 = FText::FromString(Result[6]);
  score7 = FText::FromString(Result[7]);
  score8 = FText::FromString(Result[8]);
  score9 = FText::FromString(Result[9]);

  easy = FText::FromString("Easy");
  easyDescript = FText::FromString("Sissy mode i.e. Communications Major");
  medium = FText::FromString("Medium");
  mediumDescript = FText::FromString("What normal people choose i.e. Business Major");
  hard = FText::FromString("Hard");
  hardDescript = FText::FromString("Your parents have very high standards i.e. EE or Bio-chem");

  speed = FText::FromString("Speed");
  speedDescript =
      FText::FromString("faster runner with questionable handeling");
  balanced = FText::FromString("Balanced");
  balancedDescript = FText::FromString("a runner with no particular abilities");
  traction = FText::FromString("Traction");
  tractionDescript =
      FText::FromString("a slower runner with excellent handling");
  /*
          hOrM will tell us what screen to build
                  defaults to main menu
                  1 = high score screen
                  2 = first tutorial screen
                  3 = second tutorial screen
                  5 = difficulty selection screen
                  6 = runner selection
  */
  switch (hOrM) {
  case 1:  // Build the high score screen
    ChildSlot
        [SNew(SOverlay) +
         SOverlay::Slot()
             .HAlign(HAlign_Fill)
             .VAlign(VAlign_Fill)[SNew(SImage).Image(BroncyImage)] +
         SOverlay::Slot()
             .HAlign(HAlign_Right)
             .VAlign(VAlign_Top)
             .Padding(ScorePadding)
                 [SNew(SVerticalBox)

                  + SVerticalBox::Slot().Padding(
                        ButtonPadding)[SNew(STextBlock)
                                           .Font(ButtonTextStyle)
                                           .Text(score0)  // these need to be
                                                          // FText not FString
                                           .Justification(ETextJustify::Center)
                                           .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score1)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score2)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score3)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score4)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score5)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score6)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score7)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score8)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                  SVerticalBox::Slot().Padding(
                      ButtonPadding)[SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(score9)  // these need to be
                                                        // FText not FString
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)]
                  // -------------------USE THIS FOR PROOF OF CONCEPT/DEBUGGING
                  // HIGH SCORES----------------- //
                  // + SVerticalBox::Slot()
                  //	.Padding(ButtonPadding)
                  //	[
                  //		SNew(SEditableTextBox)
                  //		.Font(ButtonTextStyle)
                  //	.Justification(ETextJustify::Center)
                  //	.BackgroundColor(FColor::Blue)
                  //	.Text(hscoreText)
                  //	.OnTextCommitted(this, &SMainMenuWidget::OnHScoreDebug)
                  //	]
                  // --------------------------------------------------------------------------------------- //

    ] +
         SOverlay::Slot()
             .HAlign(HAlign_Right)
             .VAlign(VAlign_Bottom)
             .Padding(HScoreContentPadding)[

                 // Title Text
                 SNew(SVerticalBox)

                 // Main Menu Button
                 + SVerticalBox::Slot().Padding(ButtonPadding)
                       [SNew(SButton)
                            .OnClicked(this,
                                       &SMainMenuWidget::OnReturnToMainClicked)
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(ReturnMain)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]

    ]

                 // Quit Game Button
                 + SVerticalBox::Slot().Padding(ButtonPadding)

                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnQuitClicked)
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(QuitText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]]];
    break;
  case 2:  // Build the first tutorial screen
    ChildSlot
        [SNew(SOverlay) +
         SOverlay::Slot()
             .HAlign(HAlign_Fill)
             .VAlign(VAlign_Fill)[SNew(SImage).Image(BroncyImage)]

         + SOverlay::Slot()
               .HAlign(HAlign_Right)
               .VAlign(VAlign_Bottom)
               .Padding(TutPadding)
                   [SNew(SVerticalBox)

                    // Show Tutorial Screen
                    + SVerticalBox::Slot().Padding(ButtonPadding)
                          [SNew(SButton)
                               .OnClicked(this,
                                          &SMainMenuWidget::OnNextTutClicked)
                               .ButtonColorAndOpacity(FColor::Blue)
                                   [SNew(STextBlock)
                                        .Font(ButtonTextStyle)
                                        .Text(NextText)
                                        .Justification(ETextJustify::Center)
                                        .ColorAndOpacity(FColor::Orange)]]] +
         SOverlay::Slot()
             .HAlign(HAlign_Left)
             .VAlign(VAlign_Bottom)
             .Padding(TutPadding)
                 [SNew(SVerticalBox)

                  // Show Tutorial Screen
                  + SVerticalBox::Slot().Padding(ButtonPadding)
                        [SNew(SButton)
                             .OnClicked(this,
                                        &SMainMenuWidget::OnReturnToMainClicked)
                             .ButtonColorAndOpacity(FColor::Blue)
                                 [SNew(STextBlock)
                                      .Font(ButtonTextStyle)
                                      .Text(BackText)
                                      .Justification(ETextJustify::Center)
                                      .ColorAndOpacity(FColor::Orange)]]]];
    break;
  case 3:  // Build the second tutorial screen
    ChildSlot
        [SNew(SOverlay) +
         SOverlay::Slot()
             .HAlign(HAlign_Fill)
             .VAlign(VAlign_Fill)[SNew(SImage).Image(BroncyImage)]

         +
         SOverlay::Slot()
             .HAlign(HAlign_Right)
             .VAlign(VAlign_Bottom)
             .Padding(TutPadding)
                 [SNew(SVerticalBox)

                  // Show Tutorial Screen
                  + SVerticalBox::Slot().Padding(ButtonPadding)
                        [SNew(SButton)
                             .OnClicked(this,
                                        &SMainMenuWidget::OnReturnToMainClicked)
                             .ButtonColorAndOpacity(FColor::Blue)
                                 [SNew(STextBlock)
                                      .Font(ButtonTextStyle)
                                      .Text(NextText)
                                      .Justification(ETextJustify::Center)
                                      .ColorAndOpacity(FColor::Orange)]]] +
         SOverlay::Slot()
             .HAlign(HAlign_Left)
             .VAlign(VAlign_Bottom)
             .Padding(TutPadding)
                 [SNew(SVerticalBox)

                  // Show Tutorial Screen
                  + SVerticalBox::Slot().Padding(ButtonPadding)
                        [SNew(SButton)
                             .OnClicked(this, &SMainMenuWidget::OnTutClicked)
                             .ButtonColorAndOpacity(FColor::Blue)
                                 [SNew(STextBlock)
                                      .Font(ButtonTextStyle)
                                      .Text(BackText)
                                      .Justification(ETextJustify::Center)
                                      .ColorAndOpacity(FColor::Orange)]]]];
    break;
  case 4:
    ChildSlot[SNew(SOverlay) +
              SOverlay::Slot()
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)[SNew(SImage).Image(BroncyImage)]

              +
              SOverlay::Slot()
                  .HAlign(HAlign_Left)
                  .VAlign(VAlign_Bottom)
                  .Padding(TutPadding)
                      [SNew(SVerticalBox)

                       // Show Tutorial Screen
                       + SVerticalBox::Slot().Padding(ButtonPadding)
                             [SNew(SButton)
                                  .OnClicked(this, &SMainMenuWidget::
                                                       OnReturnToMainClicked)
                                  .ButtonColorAndOpacity(FColor::Blue)
                                      [SNew(STextBlock)
                                           .Font(ButtonTextStyle)
                                           .Text(BackText)
                                           .Justification(ETextJustify::Center)
                                           .ColorAndOpacity(FColor::Orange)]]]];
    break;

  case 5:  // Build the difficulty selection screen
    ChildSlot
        
        [SNew(SOverlay) +
             SOverlay::Slot()
                 .HAlign(HAlign_Fill)
                 .VAlign(VAlign_Fill)[SNew(SImage).Image(BroncyImage)] +
             //Easy Button
             SOverlay::Slot()
                 .HAlign(HAlign_Center)
                 .VAlign(VAlign_Center)
                 .Padding(DifficultyPadding)
                 [SNew(SHorizontalBox)
                     + SHorizontalBox::Slot().Padding(ButtonPadding)
                     [SNew(SButton)
                        .OnClicked(this, &SMainMenuWidget::OnPlayClicked, FName(TEXT("Easy")))
                        .ButtonColorAndOpacity(FColor::Blue)
                         [SNew(SVerticalBox)
                          + SVerticalBox::Slot()
                                [SNew(STextBlock)
                                    .Font(ButtonTextStyle)
                                    .Text(easy)
                                    .Justification(ETextJustify::Center)
                                    .ColorAndOpacity(FColor::Orange)]
                          + SVerticalBox::Slot().Padding(0.f, 50.f)
                                [SNew(STextBlock)
                                    .Font(ButtonTextStyle)
                                    .Text(easyDescript)
                                    .Justification(ETextJustify::Center)
                                    .ColorAndOpacity(FColor::Orange)
                                    .AutoWrapText(true)
                                    .MinDesiredWidth(600.f)]
                         ]
                    ]
                    + SHorizontalBox::Slot().Padding(ButtonPadding)
                    //Medium Button
                    [SNew(SButton)
                          .OnClicked(this, &SMainMenuWidget::OnPlayClicked, FName(TEXT("Medium")))
                          .ButtonColorAndOpacity(FColor::Blue)
                              [SNew(SVerticalBox) +
                               SVerticalBox::Slot()
                                   [SNew(STextBlock)
                                        .Font(ButtonTextStyle)
                                        .Text(medium)
                                        .Justification(ETextJustify::Center)
                                        .ColorAndOpacity(FColor::Orange)]
                               + SVerticalBox::Slot()
                                   [SNew(STextBlock)
                                        .Font(ButtonTextStyle)
                                        .Text(mediumDescript)
                                        .Justification(ETextJustify::Center)
                                        .ColorAndOpacity(FColor::Orange)
                                        .AutoWrapText(true)
                                        .MinDesiredWidth(600.f)]
                               ]
                    ]
                    +  SHorizontalBox::Slot().Padding(ButtonPadding)
                    //Hard Button
                    [SNew(SButton)
                          .OnClicked(this, &SMainMenuWidget::OnPlayClicked, FName(TEXT("Hard")))
                          .ButtonColorAndOpacity(FColor::Blue)
                              [SNew(SVerticalBox) +
                               SVerticalBox::Slot()
                                   [SNew(STextBlock)
                                        .Font(ButtonTextStyle)
                                        .Text(hard)
                                        .Justification(ETextJustify::Center)
                                        .ColorAndOpacity(FColor::Orange)] +
                                SVerticalBox::Slot()
                                   [SNew(STextBlock)
                                        .Font(ButtonTextStyle)
                                        .Text(hardDescript)
                                        .Justification(ETextJustify::Center)
                                        .ColorAndOpacity(FColor::Orange)
                                        .AutoWrapText(true)
                                        .MinDesiredWidth(600.f)]
                              ]
                    ]
                ] +
                SOverlay::Slot()
                    .HAlign(HAlign_Right)
                    .VAlign(VAlign_Bottom)
                    .Padding(HScoreContentPadding)[
                        // Title Text
                        SNew(SVerticalBox)
                        // Main Menu Button
                        + SVerticalBox::Slot().Padding(ButtonPadding)
                            [SNew(SButton)
                                .OnClicked(this, &SMainMenuWidget::OnReturnToMainClicked)
                                .ButtonColorAndOpacity(FColor::Blue)
                                    [SNew(STextBlock)
                                        .Font(ButtonTextStyle)
                                        .Text(ReturnMain)
                                        .Justification(ETextJustify::Center)
                                        .ColorAndOpacity(FColor::Orange)]

                            ] +
                            // Quit Game Button
                            SVerticalBox::Slot().Padding(ButtonPadding)
                                [SNew(SButton)
                                    .OnClicked(this, &SMainMenuWidget::OnQuitClicked)
                                    .ButtonColorAndOpacity(FColor::Blue)
                                        [SNew(STextBlock)
                                                .Font(ButtonTextStyle)
                                                .Text(QuitText)
                                                .Justification(ETextJustify::Center)
                                                .ColorAndOpacity(FColor::Orange)]
                                ]
                    ]
        ];
    break;
  case 6:  // runner selection screen
    ChildSlot

        [SNew(SOverlay) +
         SOverlay::Slot()
             .HAlign(HAlign_Fill)
             .VAlign(VAlign_Fill)[SNew(SImage).Image(BroncyImage)] +
         // speed
         SOverlay::Slot()
             .HAlign(HAlign_Center)
             .VAlign(VAlign_Center)
             .Padding(SelectionPadding)
                 [SNew(SHorizontalBox) +
                  SHorizontalBox::Slot().Padding(ButtonPadding)
                      [SNew(SButton)
                           .OnClicked(this, &SMainMenuWidget::OnPlayClicked,
                                      FName(TEXT("speed")))
                           .ButtonColorAndOpacity(FColor::Blue)
                               [SNew(SVerticalBox) +
                                SVerticalBox::Slot()
                                    [SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(speed)
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                                SVerticalBox::Slot().Padding(0.f, 50.f)
                                    [SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(speedDescript)
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)
                                         .AutoWrapText(true)
                                         .MinDesiredWidth(600.f)]]] +
                  SHorizontalBox::Slot().Padding(ButtonPadding)
                      // balanced
                      [SNew(SButton)
                           .OnClicked(this, 
                               &SMainMenuWidget::OnPlayClicked,
                                      FName(TEXT("balanced")))
                           .ButtonColorAndOpacity(FColor::Blue)
                               [SNew(SVerticalBox) +
                                SVerticalBox::Slot()
                                    [SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(balanced)
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                                SVerticalBox::Slot()
                                    [SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(balancedDescript)
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)
                                         .AutoWrapText(true)
                                         .MinDesiredWidth(600.f)]]
                      ] +
                  SHorizontalBox::Slot().Padding(ButtonPadding)
                      // traction
                      [SNew(SButton)
                           .OnClicked(this, &SMainMenuWidget::OnPlayClicked,
                                      FName(TEXT("traction")))
                           .ButtonColorAndOpacity(FColor::Blue)
                               [SNew(SVerticalBox) +
                                SVerticalBox::Slot()
                                    [SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(traction)
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)] +
                                SVerticalBox::Slot()
                                    [SNew(STextBlock)
                                         .Font(ButtonTextStyle)
                                         .Text(tractionDescript)
                                         .Justification(ETextJustify::Center)
                                         .ColorAndOpacity(FColor::Orange)
                                         .AutoWrapText(true)
                                         .MinDesiredWidth(600.f)]]]] +
         SOverlay::Slot()
             .HAlign(HAlign_Right)
             .VAlign(VAlign_Bottom)
             .Padding(HScoreContentPadding)[
                 // Title Text
                 SNew(SVerticalBox)
                 // Main Menu Button
                 + SVerticalBox::Slot().Padding(ButtonPadding)
                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnPlayClicked, FName(TEXT("balanced")))
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(ReturnMain)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]] +
                 // Quit Game Button
                 SVerticalBox::Slot().Padding(ButtonPadding)
                     [SNew(SButton)
                          .OnClicked(this, &SMainMenuWidget::OnQuitClicked)
                          .ButtonColorAndOpacity(FColor::Blue)
                              [SNew(STextBlock)
                                   .Font(ButtonTextStyle)
                                   .Text(QuitText)
                                   .Justification(ETextJustify::Center)
                                   .ColorAndOpacity(FColor::Orange)]]]];
    break;
  default:  // Build the main menu
    ChildSlot
        [SNew(SOverlay) +
         SOverlay::Slot()
             .HAlign(HAlign_Fill)
             .VAlign(VAlign_Fill)[SNew(SImage).Image(BroncyImage)] +
         SOverlay::Slot()
             .HAlign(HAlign_Left)
             .VAlign(VAlign_Bottom)
             .Padding(LogoPadding)
                 [SNew(SVerticalBox) +
                  SVerticalBox::Slot().Padding(ButtonPadding)
                      [SNew(SButton)
                           .OnClicked(this, &SMainMenuWidget::OnCredClicked)
                           .ButtonColorAndOpacity(
                               FColor::Blue)[SNew(SImage).Image(SDPLogo)]]] +
         SOverlay::Slot()
             .HAlign(HAlign_Right)
             .VAlign(VAlign_Top)
             .Padding(ContentPadding)[

                 SNew(SVerticalBox)

                 // Show Tutorial Screen
                 + SVerticalBox::Slot().Padding(ButtonPadding)
                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnTutClicked)
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(tutText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]
                 // Play main game
                 + SVerticalBox::Slot().Padding(ButtonPadding)
                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnPlayClicked, FName(TEXT("Medium")), FName(TEXT("balanced")))
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(PlayText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]
                 // Play during day text
                 + SVerticalBox::Slot().Padding(ButtonPadding)
                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnFreePlayClicked, FName(TEXT("Medium")), FName(TEXT("balanced")))
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(FreePlayText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]
                 // Difficulty Selection
                 + SVerticalBox::Slot().Padding(ButtonPadding)
                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnDifficultyClicked)
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(SelectDifficultyText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]
                 // reunner Selection
                 + SVerticalBox::Slot().Padding(ButtonPadding)
                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::onRunnerSelectionClicked)
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(SelectRunnerText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]
                 // High Score Button
                 + SVerticalBox::Slot().Padding(ButtonPadding)

                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnHScoreClicked)
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(HighScoreText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]

                 // Quit Game Button Text
                 + SVerticalBox::Slot().Padding(ButtonPadding)

                       [SNew(SButton)
                            .OnClicked(this, &SMainMenuWidget::OnQuitClicked)
                            .ButtonColorAndOpacity(FColor::Blue)
                                [SNew(STextBlock)
                                     .Font(ButtonTextStyle)
                                     .Text(QuitText)
                                     .Justification(ETextJustify::Center)
                                     .ColorAndOpacity(FColor::Orange)]]]];
    break;
  }
}

#undef LOCTEXT_NAMESPACE
