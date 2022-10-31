// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BroncoSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BRONCODROME_API UBroncoSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int score;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int mapsBeaten;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FName difficultySetting;  

	UBroncoSaveGame();
};
