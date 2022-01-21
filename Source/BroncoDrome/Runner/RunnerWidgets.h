// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "RunnerWidgets.generated.h"

/**
 * 
 */
UCLASS()
class BRONCODROME_API URunnerWidgets: public UUserWidget
{
	GENERATED_BODY()

public:		// Constructors

	// UUserWidget does not have a constructor :(
	// Instead, NativePreConstruct() and NativeConstruct() overrides to init variables
	UPROPERTY(BlueprintReadOnly)
	int playerScore = 0; 

	UPROPERTY(BlueprintReadOnly)
	float healthFloat = 1.0; 

	UPROPERTY(BlueprintReadOnly)
	float speed = 0.0;


protected:	// Overrides

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:	// Members



public:		// Interface

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // unused but it causes an error when I remove it 
	void OnSetScore(int score);

	void increaseScoreVar(int score); // use this one 

	void SetHealth(int health);
	void SetSpeed(float newSpeed); 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRenderLockOnReticle(FVector worldSpace, bool hide);

};
