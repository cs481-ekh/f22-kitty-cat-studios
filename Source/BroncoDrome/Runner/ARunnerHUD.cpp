// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "ARunnerHUD.h"

#include "GameFramework/GameUserSettings.h"

ARunnerHUD::ARunnerHUD()
{

}

void ARunnerHUD::BeginPlay()
{
	Super::BeginPlay();

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
	if (!paused) {
		paused = true; 
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		m_PauseWidgets->AddToViewport();
	}
	else {
		paused = false; 
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		m_PauseWidgets->RemoveFromViewport();
	}
	
}