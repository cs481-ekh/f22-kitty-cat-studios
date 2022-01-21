// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidgets.generated.h"

/**
 * 
 */
UCLASS()
class BRONCODROME_API UPauseWidgets : public UUserWidget
{
	GENERATED_BODY()

public:		// Interface

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUnpause();
	
};
