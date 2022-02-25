// // Copyright (C) Team Gregg 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinWidget.generated.h"

UCLASS()
class BRONCODROME_API UWinWidget: public UUserWidget
{
	GENERATED_BODY()

public:		// Constructors

	// UUserWidget does not have a constructor :(
	// Instead, NativePreConstruct() and NativeConstruct() overrides to init variables
	//UPROPERTY(BlueprintReadOnly)
	//int Win = 0;


protected:	// Overrides

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:	// Members

    //UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // unused but it causes an error when I remove it 
	

public:		// Interface

    //UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // unused but it causes an error when I remove it 
    //void YouWin();      
};