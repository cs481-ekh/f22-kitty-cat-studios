// // Copyright (C) Team Gregg 2022. All Rights Reserved.

#include "LoseWidget.h"

void ULoseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();


}

void ULoseWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void ULoseWidget::hScoreSubmit(FText initials) 
{
	FString pHolder = initials.ToString();
	int placeable = 0;
	//IF the player name is too short
	if (pHolder.Len() < 3 || pHolder.IsEmpty()) {
		//THEN print an error
		pText->SetText(FText::FromString("NAME NOT LONG ENOUGH. 3 DIGITS PLEASE"));
	}
	//IF the player name is too long
	else if (pHolder.Len() > 3) {
		//THEN print an error
		pText->SetText(FText::FromString("NAME TOO LONG. 3 DIGITS PLEASE"));
	}
	//ELSE the player name is juuuuuuust right
	else {
		pHolder = pHolder.ToUpper();
		placeable = 1;
	}
	//Build the potential high score entry
	pHolder.Append(" ");
	pHolder.Append(FString::FromInt(pscore));
	//UE_LOG(LogTemp, Warning, TEXT("Potential Entry: %s"), *pHolder);
	//Check if the player score should even be on the board
	FString checkScore = Result[9].RightChop(4);
	int checkInt = FCString::Atoi(*checkScore);
	//IF the player score is at least larger than the lowest score
	UE_LOG(LogTemp, Warning, TEXT("checkInt: %d pscore %d placeable: %d"), checkInt, pscore, placeable);
	if (checkInt < pscore && placeable) {
		//Get the current scores and compare them with the player score
		int placed = 0;
		for (int i = 8; i >= 0; i--) {
			FString currScore = Result[i].RightChop(4);
			int currInt = FCString::Atoi(*currScore);
			//IF player score is less than the current score 
			if (currInt > pscore) {
				//THEN we need to place the player score into the previous slot
				//NOTE: Since we've already checked if the players score should be on
				//      the board we can assume this will work
				Result[i + 1] = pHolder;
				placed = 1;
				break;
			}
			//ELSE shift the scores down
			else
				Result[i + 1] = Result[i];
		}
		//IF the score is larger than all other scores
		if (!placed) {
			//THEN put it in the highest score
			Result[0] = pHolder;
		}
		UE_LOG(LogTemp, Warning, TEXT("UPDATING SCORES"));
		//UPDATE THE SCORE FILE
		FString file = FPaths::ProjectConfigDir();
		file.Append(TEXT("highScores.txt"));
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		if (FileManager.FileExists(*file)) {
			if (FFileHelper::SaveStringArrayToFile(Result, *file)) {
				UE_LOG(LogTemp, Warning, TEXT("Score Array Updated"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not update Scores"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
		}
		pText->SetText(FText::FromString("HIGH SCORE ADDED :)"));
		sBox->SetIsEnabled(false);
	}
}

void ULoseWidget::setScore(int score) 
{
	pscore = score;
}

void ULoseWidget::showHScore(UVerticalBox* scoreBox, UTextBlock* pleaseText, UTextBlock* totalScore) 
{
	sBox = scoreBox;
	pText = pleaseText;
	//Need to pull the current high scores
	FString file = FPaths::ProjectConfigDir();
	file.Append(TEXT("highScores.txt"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	if (FileManager.FileExists(*file)) {
		if (FFileHelper::LoadFileToStringArray(Result, *file)) {
			//UE_LOG(LogTemp, Warning, TEXT("Score Array Loaded"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not Load Scores"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
	}

	FString checkScore = Result[9].RightChop(4);
	int checkInt = FCString::Atoi(*checkScore);
	//IF the player score is at least larger than the lowest score
	if (checkInt < pscore) {
		FString totScore = "Total Score: ";
		totScore.AppendInt(pscore);
		totalScore->SetText(FText::FromString(totScore));
		//Need to make high score buttons visable and enabled
		scoreBox->SetVisibility(ESlateVisibility::Visible);
		scoreBox->SetIsEnabled(true);
	}
}

void ULoseWidget::PlayFadeInAnimation_Implementation() {}
