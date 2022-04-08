// // Copyright (C) Team Gregg 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/SlateWrapperTypes.h"
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
	
	//player score
	UPROPERTY(BlueprintReadOnly)
		int pscore = 0;
	//Array for scores
	UPROPERTY()
		TArray<FString> Result;
	//Vertical box for the high score on the win screen
	UPROPERTY()
		UVerticalBox* sBox;
	UPROPERTY()
		UTextBlock* pText;

protected:	// Overrides

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:	// Members

    //UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // unused but it causes an error when I remove it 
	

public:		// Interface

    //UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // unused but it causes an error when I remove it 
    //void YouWin();      
	
	//Function to submit the new high score (if applicable)
	UFUNCTION(BlueprintCallable, Category = UWinWidget)
		void hScoreSubmit(FText initials);

	//Function to set the current players score
	UFUNCTION(BlueprintCallable, Category = UWinWidget)
		void setScore(int score);

	UFUNCTION(BlueprintCallable, Category = UWinWidget)
		void showHScore(UVerticalBox* scoreBox, UTextBlock* pleaseText);

};
