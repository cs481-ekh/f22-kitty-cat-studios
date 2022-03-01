// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "MenuHUD.h"
#include "SMainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMenuHUD::AMenuHUD() {
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> BroncyImagePath(TEXT("Texture2D'/Game/Assets/Screenshots/broncy.broncy'"));
	BroncyImage = BroncyImagePath.Object; //This is the image for the main menu
	static ConstructorHelpers::FObjectFinder<UTexture2D> HScoreImagePath(TEXT("Texture2D'/Game/Assets/Screenshots/hscore.hscore'"));
	HScoreImage = HScoreImagePath.Object; //This is the image for the high score screen
}

void AMenuHUD::BeginPlay()
{

	Super::BeginPlay();

	ShowMenu(0); //0 makes it show the main menu screen intially


}

void AMenuHUD::ShowMenu(int i)
{
	mainOrHScore = i; //SMainMenuWidget needs to know what this number is -> 0=main menu | 1=high score
	if (GEngine && GEngine->GameViewport)
	{
		//IF we want to show the main menu
		if(!i)
			MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(BroncyImage); //This will load SMainMenuWidget with the main menu image
		//ELSE IF we want show the high score screen
		else if (i)
			MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(HScoreImage); //This will load SMainMenuWidget with the high score image
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}

}

void AMenuHUD::RemoveMenu()
{
	if (GEngine && GEngine->GameViewport && MenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}
