// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "RunnerWidgets.h"
#include "PauseWidgets.h"
#include "WinWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ARunnerHUD.generated.h"

/**
 * 
 */
UCLASS()
class BRONCODROME_API ARunnerHUD : public AHUD
{
	GENERATED_BODY()

public:	// Constructors

	ARunnerHUD();

protected:	// Overloads

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

public:		// Blueprint variables

	UPROPERTY(EditDefaultsOnly, Category = "RunnerWidgets")
	TSubclassOf<UUserWidget> RunnerWidgetsClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pause")
	TSubclassOf<UUserWidget> PauseWidgetsClass;

	//WinBP Variables
	UPROPERTY(EditDefaultsOnly, Category = "WinWidget")
	TSubclassOf<UUserWidget> WinWidgetClass;

	bool paused = false; 

private:	// Members

	URunnerWidgets* m_Widgets;
	UPauseWidgets* m_PauseWidgets; 
	//Widget for the win screen
	UWinWidget* m_WinWidget;

public:		// Interface

	inline void AddToScore(int score) { m_Widgets->increaseScoreVar(score); } // use this one
	inline void SetScore(int score) { m_Widgets->OnSetScore(score); } // unused but it causes an error when I remove, so I'm leaving it 
	inline void SetHealth(int health) { m_Widgets->SetHealth(health); }
	inline void SetSpeed(float speed) { m_Widgets->SetSpeed(speed); }
	inline void RenderLockOnReticle(FVector worldSpace, bool hide) { m_Widgets->OnRenderLockOnReticle(worldSpace, hide); }

public: 
	void Pause();
	void HideHUD(bool option);
	//For Win Condition
	void YouWin();
};
