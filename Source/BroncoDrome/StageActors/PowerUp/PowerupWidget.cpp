// // Copyright (C) Team Kitty Cat 2022. All Rights Reserved.


#include "PowerupWidget.h"

void UPowerupWidget::NativePreConstruct() { Super::NativePreConstruct(); }

void UPowerupWidget::NativeConstruct() {
  Super::NativeConstruct();

  powerupName = FString("");
}

void UPowerupWidget::SetPowerupText(FString powerupString) {
  powerupName = powerupString;
}

FString UPowerupWidget::GetPowerupText() { return powerupName; }

// This method is implemented inside the actual blueprint instance. Setting up the method
// like this allows the blueprint method to be called by a C++ script. When calling the method
// "Implementation" is not included. See this article for more information
// https://shahriyarshahrabi.medium.com/using-blueprintnativeevent-with-inheritance-for-blueprint-and-cpp-configurable-functions-25ce94e20f34
void UPowerupWidget::PlayFadeInAnimation_Implementation() {} 
