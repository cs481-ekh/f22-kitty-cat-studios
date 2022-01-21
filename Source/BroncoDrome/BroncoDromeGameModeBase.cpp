// Copyright Epic Games, Inc. All Rights Reserved.


#include "BroncoDromeGameModeBase.h"
#include "MainMenuPlayerController.h"
#include "MenuHUD.h"

ABroncoDromeGameModeBase::ABroncoDromeGameModeBase() {


	PlayerControllerClass = AMainMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();

}