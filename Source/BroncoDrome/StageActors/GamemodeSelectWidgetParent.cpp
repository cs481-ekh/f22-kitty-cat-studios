// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.

#include "GamemodeSelectWidgetParent.h"
#include "Kismet/GameplayStatics.h"
#include "BroncoDrome/BroncoSaveGame.h"

void UGamemodeSelectWidgetParent::NativePreConstruct() { Super::NativePreConstruct(); }

void UGamemodeSelectWidgetParent::NativeConstruct() {
	Super::NativeConstruct();
	levelSelected = 0;
	runnerSelected = 0;
	difficultySelected = 0;
	gamemodeSelected = 0;
}

void UGamemodeSelectWidgetParent::SetSelectedLevel(int levelIndex) {
	levelSelected = levelIndex;
}

void UGamemodeSelectWidgetParent::SetSelectedRunner(int runnerIndex)
{
	runnerSelected = runnerIndex;
}

void UGamemodeSelectWidgetParent::SetSelectedDifficulty(int difficultyIndex)
{
	difficultySelected = difficultyIndex;
}

void UGamemodeSelectWidgetParent::SetSelectedGamemode(int gamemodeIndex)
{
	gamemodeSelected = gamemodeIndex;
}

FName UGamemodeSelectWidgetParent::GetGamemodeFName() {
	switch (gamemodeSelected) {
		case 1: return FName(TEXT("Standard"));
		case 2: return FName(TEXT("Survival"));
		case 3: return FName(TEXT("Freeplay"));
		default: return FName(TEXT("Standard"));
	}
}


FName UGamemodeSelectWidgetParent::GetLevelFName() {
	switch (levelSelected) {
		case 1: return FName(TEXT("Broncodrome_Day"));
		case 2: return FName(TEXT("Broncodrome_Night"));
		case 3: return FName(TEXT("SUB_Map_Day"));
		case 4: return FName(TEXT("SUB_Map_Night"));
		default: return FName(TEXT("Broncodrome_Day"));
	}
}

FName UGamemodeSelectWidgetParent::GetRunnerFName() {
	switch (runnerSelected) {
		case 1: return FName(TEXT("balanced"));
		case 2: return FName(TEXT("speed"));
		case 3: return FName(TEXT("traction"));
		default: return FName(TEXT("balanced"));
	}
}

FName UGamemodeSelectWidgetParent::GetDifficultyFName() {
	switch (difficultySelected) {
		case 1: return FName(TEXT("Easy"));
		case 2: return FName(TEXT("Medium"));
		case 3: return FName(TEXT("Hard"));
		default: return FName(TEXT("Medium"));
	}
}

void UGamemodeSelectWidgetParent::SetGamemodeDisplayString() {
	switch (gamemodeSelected) {
		case 1: 
			gamemodeSelectedDisplay = FName(TEXT("Gamemode: Standard"));
			break;
		case 2: 
			gamemodeSelectedDisplay = FName(TEXT("Gamemode: Survival"));
			break;			
		case 3: 
			gamemodeSelectedDisplay = FName(TEXT("Gamemode: Freeplay"));
			break;				
		default: 
			gamemodeSelectedDisplay = FName(TEXT("Gamemode: Standard"));
			break;		
	}
}


void UGamemodeSelectWidgetParent::SetLevelDisplayString() {
	switch (levelSelected) {
		case 1: 
			levelSelectedDisplay = FName(TEXT("Map: Bronco Stadium Day"));
			break;
		case 2: 
			levelSelectedDisplay = FName(TEXT("Map: Bronco Stadium Night"));
			break;			
		case 3: 
			levelSelectedDisplay = FName(TEXT("Map: SUB Day"));
			break;			
		case 4: 
			levelSelectedDisplay = FName(TEXT("Map: SUB Night"));
			break;			
		default: 
			levelSelectedDisplay = FName(TEXT("Map: Bronco Stadium Day"));
			break;			
	}
}

void UGamemodeSelectWidgetParent::SetRunnerDisplayString() {
	switch (runnerSelected) {
		case 1: 
			runnerSelectedDisplay = FName(TEXT("Runner: Balanced"));
			break;
		case 2: 
			runnerSelectedDisplay = FName(TEXT("Runner: Speed"));
			break;			
		case 3: 
			runnerSelectedDisplay = FName(TEXT("Runner: Traction"));
			break;			
		default: 
			runnerSelectedDisplay = FName(TEXT("Runner: Balanced"));
			break;			
	}
}

void UGamemodeSelectWidgetParent::SetDifficultyDisplayString() {
	switch (difficultySelected) {
		case 1: 
			difficultySelectedDisplay = FName(TEXT("Difficulty: Rookie"));
			break;
		case 2: 
			difficultySelectedDisplay = FName(TEXT("Difficulty: Normal"));
			break;			
		case 3: 
			difficultySelectedDisplay = FName(TEXT("Difficulty: Bronco"));
			break;			
		default: 
			difficultySelectedDisplay = FName(TEXT("Difficulty: Normal"));
			break;	
	}
}

void UGamemodeSelectWidgetParent::SetDisplayStrings() {
	SetGamemodeDisplayString();
	SetLevelDisplayString();
	SetRunnerDisplayString();
	SetDifficultyDisplayString();
}

void UGamemodeSelectWidgetParent::InitSave() {
  // Create and save the initial values. Nothing else is needed here "save" will
  // have the name "curr" for it's save slot.
  if (UBroncoSaveGame *save = Cast<UBroncoSaveGame>(UGameplayStatics::CreateSaveGameObject(UBroncoSaveGame::StaticClass()))) {
    save->score = 0;
	if (gamemodeSelected != 1) { // survival or freeplay (only play one game) TODO: refactor mapsBeaten and change the way maps are progressed
		save->mapsBeaten = 3;
	} else {
		save->mapsBeaten = -1; // standard game (progress through each level)
	}
	save->gamemodeSelection = GetGamemodeFName();
    save->runnerSelection = GetRunnerFName();
    save->difficultySetting = GetDifficultyFName();

    UGameplayStatics::SaveGameToSlot(save, save->SaveName, 0);
  }
  else {
	  UE_LOG(LogTemp, Error, TEXT("Could not initialize save game!"));
  }
}

void UGamemodeSelectWidgetParent::PlayLevel() {
	UGameplayStatics::OpenLevel(GWorld, GetLevelFName());
}

void UGamemodeSelectWidgetParent::ReturnToMainMenu() {
  UGameplayStatics::OpenLevel(GWorld, "MenuLevel");
}

void UGamemodeSelectWidgetParent::PlayFadeInAnimation_Implementation() {}
