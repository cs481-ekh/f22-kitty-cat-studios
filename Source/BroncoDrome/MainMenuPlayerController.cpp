// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "MainMenuPlayerController.h"
#include "MenuHUD.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{

}

void AMainMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent) {
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AMainMenuPlayerController::OpenMenu);
	}
}

void AMainMenuPlayerController::OpenMenu()
{
	if (AMenuHUD* MenuHUD = Cast<AMenuHUD>(GetHUD()))
	{
		MenuHUD->ShowMenu();
	}
}