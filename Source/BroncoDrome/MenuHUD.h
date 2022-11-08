// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class BRONCODROME_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	TSharedPtr<class SMainMenuWidget> MenuWidget;
	TSharedPtr<class SWidget> MenuWidgetContainer;

	virtual void BeginPlay() override;

public:
	int mainOrHScore;
	void ShowMenu(int i);
	void RemoveMenu();
	AMenuHUD();


private:
	UPROPERTY()
		UTexture2D* BroncyImage;
	UPROPERTY()
		UTexture2D* HScoreImage;
	UPROPERTY()
		UTexture2D* tut1Image;
	UPROPERTY()
		UTexture2D* tut2Image;
	UPROPERTY()
		UTexture2D* sdpLogo;
	UPROPERTY()
		UTexture2D* credits;
    UPROPERTY()
        UTexture2D *difficultyImage;
    UPROPERTY()
    UTexture2D *selectionImage;
};
