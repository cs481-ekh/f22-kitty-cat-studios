// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.

#include "LevelSelectWidget.h"
#include "Kismet/GameplayStatics.h"

void ULevelSelectWidget::NativePreConstruct() { Super::NativePreConstruct(); }

void ULevelSelectWidget::NativeConstruct() {
  Super::NativeConstruct();
  levelSelected = 0;
}

void ULevelSelectWidget::SetSelectedLevel(int levelIndex) {
  levelSelected = levelIndex;
}

void ULevelSelectWidget::ReturnToMainMenu() {
  UGameplayStatics::OpenLevel(GWorld, "MenuLevel");
}
