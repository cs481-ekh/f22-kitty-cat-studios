// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BRONCODROME_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	AMainMenuPlayerController();

	virtual void SetupInputComponent() override;

	void OpenMenu();


};
