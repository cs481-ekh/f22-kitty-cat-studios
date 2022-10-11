// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "RunnerWidgets.h"
#include "PauseWidgets.h"
#include "WinWidget.h"
#include "LoseWidget.h"
#include "../StageActors/PowerUp/PowerupWidget.h"
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
	UPROPERTY(EditDefaultsOnly, Category = "RunnerWidgets")
		TSubclassOf<UUserWidget> RunnerWidgetsClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pause")
		TSubclassOf<UUserWidget> PauseWidgetsClass;

	//WinBP Variables
	UPROPERTY(EditDefaultsOnly, Category = "WinWidget")
		TSubclassOf<UUserWidget> WinWidgetClass;

	//LoseBP Variables
	UPROPERTY(EditDefaultsOnly, Category = "LoseWidget")
		TSubclassOf<UUserWidget> LoseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "PowerupWidget")
		TSubclassOf<UUserWidget> PowerupWidgetClass;
	UPROPERTY()
		UWorld* world;

protected:	// Overloads

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

public:		// Blueprint variables

	UPROPERTY()
		bool paused = false; 

private:	// Members

	UPROPERTY()
		URunnerWidgets* m_Widgets;
	UPROPERTY()
		UPauseWidgets* m_PauseWidgets; 
	//Widget for the win screen
	UPROPERTY()
		UWinWidget* m_WinWidget;
	//Widget for the Lose Screen
	UPROPERTY()
		ULoseWidget* m_LoseWidget;

	UPROPERTY()
		UPowerupWidget* m_PowerupWidget;

		

public:		// Interface

	inline void AddToScore(int score) { m_Widgets->increaseScoreVar(score); } // use this one
	inline void SetScore(int score) { m_Widgets->OnSetScore(score); } // unused but it causes an error when I remove, so I'm leaving it 
	inline void SetHealth(int health) { m_Widgets->SetHealth(health); }
	inline void SetSpeed(float speed) { m_Widgets->SetSpeed(speed); }
	inline void SetDead(bool newDead) { m_Widgets->SetDead(newDead); }
	inline void SetTimeLeft(int newTime) { m_Widgets->SetTimeLeft(newTime); }
	inline void DecrementLivesLeft() { m_Widgets->DecrementLivesLeft(); }
	inline void SetEnemiesLeft(int newAmount) { m_Widgets->SetEnemiesLeft(newAmount); SetAnemonies(newAmount); }
	inline void DecrementEnemiesLeft(void) { m_Widgets->DecrementEnemiesLeft(); DecrementAnemonies(); }
	inline void SetGameTimeRemaining(int currentGameTime) {m_Widgets->SetGameTimeRemaining(currentGameTime);}
	inline void RenderLockOnReticle(FVector worldSpace, bool hide) { m_Widgets->OnRenderLockOnReticle(worldSpace, hide); }

public: 
	void Pause();
	void HideHUD(bool option);
	//For Win Condition
	void YouWin();
	//For Loss Condition
	void YouLose();
	void SetAnemonies(int anemonies);
	void DecrementAnemonies();
	int getLives();
	int getEnemiesLeft();
	void ShowPowerupWidget(FString powerupText);
};
