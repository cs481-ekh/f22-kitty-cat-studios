// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.

#include "GamemodeSelectWidget.h"
#include "Kismet/GameplayStatics.h"

void UGamemodeSelectWidget::NativePreConstruct() { Super::NativePreConstruct(); }

void UGamemodeSelectWidget::NativeConstruct() {
  Super::NativeConstruct();
  levelSelected = 0;
}

void UGamemodeSelectWidget::SetSelectedLevel(int levelIndex) {
  levelSelected = levelIndex;
}

void UGamemodeSelectWidget::PlayLevel() {
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

void UGamemodeSelectWidget::ReturnToMainMenu() {
  UGameplayStatics::OpenLevel(GWorld, "MenuLevel");
}

void UGamemodeSelectWidget::PlayFadeInAnimation_Implementation() {}
