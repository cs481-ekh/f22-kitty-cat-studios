// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.

#include "GamemodeSelectWidgetParent.h"
#include "Kismet/GameplayStatics.h"

void UGamemodeSelectWidgetParent::NativePreConstruct() { Super::NativePreConstruct(); }

void UGamemodeSelectWidgetParent::NativeConstruct() {
  Super::NativeConstruct();
  levelSelected = 0;
}

void UGamemodeSelectWidgetParent::SetSelectedLevel(int levelIndex) {
  levelSelected = levelIndex;
}

void UGamemodeSelectWidgetParent::PlayLevel() {
	//If more levels are added, add their paths here
	switch (levelSelected) {
		case 1:
          UGameplayStatics::OpenLevel(GWorld, "Broncodrome_Day");
          break;
		case 2:
		  UGameplayStatics::OpenLevel(GWorld, "Broncodrome_Night");
		  break;
		case 3:
		  //UGameplayStatics::OpenLevel(GWorld, "SUB_Map_Day");
		  break;
		case 4:
		  //UGameplayStatics::OpenLevel(GWorld, "SUB_Map_Night");
		  break;
  }
}

void UGamemodeSelectWidgetParent::ReturnToMainMenu() {
  UGameplayStatics::OpenLevel(GWorld, "MenuLevel");
}

void UGamemodeSelectWidgetParent::PlayFadeInAnimation_Implementation() {}
