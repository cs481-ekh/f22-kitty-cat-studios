// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "ARunnerHUD.h"

#include "GameFramework/GameUserSettings.h"

ARunnerHUD::ARunnerHUD()
{

}

void ARunnerHUD::BeginPlay()
{
	Super::BeginPlay();
	paused = false;
	world = GetWorld();
	if(world == NULL)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not load world"));
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (RunnerWidgetsClass)
	{
		m_Widgets = CreateWidget<URunnerWidgets>(GetWorld(), RunnerWidgetsClass);

		if (m_Widgets)
		{
			m_Widgets->AddToViewport();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not create Widgets"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find TSubclassOf<UUserWidget>"));
	}

	if (PauseWidgetsClass) {
		m_PauseWidgets = CreateWidget<UPauseWidgets>(GetWorld(), PauseWidgetsClass); 
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find TSubclassOf<UUserWidget>"));
	}
	//Enable Win Widget
	if (WinWidgetClass) {
		m_WinWidget = CreateWidget<UWinWidget>(GetWorld(), WinWidgetClass);
		if(m_WinWidget == NULL)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Win Widget not verified"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find TSubclassOf<UUserWidget>"));
	}
	//Lose Widget Enabling
	if (LoseWidgetClass) {
		m_LoseWidget = CreateWidget<ULoseWidget>(GetWorld(), LoseWidgetClass); 
		if(m_LoseWidget == NULL)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lose Widget not verified"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find TSubclassOf<UUserWidget>"));
	}
}

void ARunnerHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ARunnerHUD::HideHUD(bool option)
{
	if(option)
		m_Widgets->SetVisibility(ESlateVisibility::Hidden);
	else
		m_Widgets->SetVisibility(ESlateVisibility::Visible);
}

void ARunnerHUD::Pause() {
	class APlayerController* Mouse;
	Mouse = GetWorld()->GetFirstPlayerController();
	
	if (!paused) {
		paused = true; 
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		Mouse->bShowMouseCursor = true;
		Mouse->bEnableClickEvents = true;
		Mouse->bEnableMouseOverEvents = true;
		m_PauseWidgets->AddToViewport();
	}
	else {
		paused = false; 
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		Mouse->bShowMouseCursor = false;
		Mouse->bEnableClickEvents = false;
		Mouse->bEnableMouseOverEvents = false;
		m_PauseWidgets->RemoveFromViewport();
	}
	
}

//Win Condition Call
void ARunnerHUD::YouWin(){
	//SHOULD display win widget, is causing an error
	class APlayerController* Mouse;
	Mouse = world->GetFirstPlayerController();
	paused = true;
	Mouse->bShowMouseCursor = true;
	Mouse->bEnableClickEvents = true;
	Mouse->bEnableMouseOverEvents = true;
	m_WinWidget->setScore(m_Widgets->getScore());
	m_WinWidget->AddToViewport();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("You Win Function Reached"), *GetDebugName(this)));
	UGameplayStatics::SetGamePaused(world, true);
}

void ARunnerHUD::YouLose() 
{
	class APlayerController* Mouse;
	Mouse = world->GetFirstPlayerController();
	paused = true;
	Mouse->bShowMouseCursor = true;
	Mouse->bEnableClickEvents = true;
	Mouse->bEnableMouseOverEvents = true;
	m_LoseWidget->setScore(m_Widgets->getScore());
	m_LoseWidget->AddToViewport();
	UGameplayStatics::SetGamePaused(world, true);
}