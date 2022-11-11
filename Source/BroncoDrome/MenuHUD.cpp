// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "MenuHUD.h"
#include "SMainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AMenuHUD::AMenuHUD() {
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> BroncyImagePath(TEXT("Texture2D'/Game/Assets/Screenshots/broncy.broncy'"));
	BroncyImage = BroncyImagePath.Object; //This is the image for the main menu
	static ConstructorHelpers::FObjectFinder<UTexture2D> HScoreImagePath(TEXT("Texture2D'/Game/Assets/Screenshots/hscore.hscore'"));
	HScoreImage = HScoreImagePath.Object; //This is the image for the high score screen
	static ConstructorHelpers::FObjectFinder<UTexture2D> Tutorial1Path(TEXT("Texture2D'/Game/Assets/Screenshots/tut1.tut1'"));
	tut1Image = Tutorial1Path.Object; //Image for the first tutorial page
	static ConstructorHelpers::FObjectFinder<UTexture2D> Tutorial2Path(TEXT("Texture2D'/Game/Assets/Screenshots/tut2.tut2'"));
	tut2Image = Tutorial2Path.Object; //Image for the second tutorial page
	static ConstructorHelpers::FObjectFinder<UTexture2D> SDPLogoPath(TEXT("Texture2D'/Game/Assets/Screenshots/sdp-logo-infinity.sdp-logo-infinity'"));
	sdpLogo = SDPLogoPath.Object; //Image for the SDP logo
	static ConstructorHelpers::FObjectFinder<UTexture2D> CreditsPath(TEXT("Texture2D'/Game/Assets/Screenshots/credits.credits'"));
	credits = CreditsPath.Object; //Image for the Credits page
	static ConstructorHelpers::FObjectFinder<UTexture2D> DifficultyImagePath(TEXT("Texture2D'/Game/Assets/Screenshots/difficultyImage.difficultyImage'"));
    difficultyImage = DifficultyImagePath.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> buttonCue(
		TEXT("'/Game/Assets/Sound/Button_Select_Cue.Button_Select_Cue'")
	);
	buttonAudioCue = buttonCue.Object;
  
  static ConstructorHelpers::FObjectFinder<UTexture2D> selectionImagePath(TEXT("Texture2D'/Game/Assets/Screenshots/selectionImage.selectionImage'"));
  selectionImage = selectionImagePath.Object;
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
		switch (i) {
			case 1:
				MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(HScoreImage); //This will load SMainMenuWidget with the high score image
				break;
			case 2:
				MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(tut1Image); //This will load SMainMenuWidget with the first tutorial image
				break;
			case 3:
				MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(tut2Image); //This will load SMainMenuWidget with the second tutorial image
				break;
			case 4:
				MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(credits); //This will load SMainMenuWidget with the credits page
				break;
			case 5:
				MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(difficultyImage); //This will load SMainMenuWidget with the difficulty selection page
				break;
			case 6:
				MenuWidget = SNew(SMainMenuWidget) .OwningHUD(this).broncyImage(selectionImage);  
                 break;
			default:
				MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(BroncyImage).sdpLogo(sdpLogo); //This will load SMainMenuWidget with the main menu image
				break;
		}
		//IF we want to show the main menu
		//if(!i)
			//MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(BroncyImage); //This will load SMainMenuWidget with the main menu image
		//ELSE IF we want show the high score screen
		//else if (i)
			//MenuWidget = SNew(SMainMenuWidget).OwningHUD(this).broncyImage(HScoreImage); //This will load SMainMenuWidget with the high score image
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
		UGameplayStatics::PlaySound2D(GetWorld(), buttonAudioCue);
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}
