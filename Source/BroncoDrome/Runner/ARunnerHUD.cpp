// // Copyright (C) Dromies 2021 + Team Gregg 2022. All Rights Reserved.


#include "ARunnerHUD.h"
#include "BroncoDrome/BroncoSaveGame.h"
#include "Sound/SoundCue.h"

#include "GameFramework/GameUserSettings.h"

int anemoniesLeft = 0;

ARunnerHUD::ARunnerHUD()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> pauseCue(
		TEXT("'/Game/Assets/Sound/Pause_Cue.Pause_Cue'")
	);
	pauseAudioCue = pauseCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> unPauseCue(
		TEXT("'/Game/Assets/Sound/UnPause_Cue.UnPause_Cue'")
	);
	unPauseAudioCue = unPauseCue.Object;
}

void ARunnerHUD::BeginPlay()
{
	Super::BeginPlay();
	paused = false;
	world = GetWorld();
	//Checks to make sure the 
	if(world == NULL)
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not load world"));
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	//Checks for each widget present in the HUD that they are loaded
	if (RunnerWidgetsClass)
	{
		m_Widgets = CreateWidget<URunnerWidgets>(GetWorld(), RunnerWidgetsClass);

		if (m_Widgets)
		{
			m_Widgets->AddToViewport();
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not create Widgets"));
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find TSubclassOf<UUserWidget>"));
	}

	if (PauseWidgetsClass) {
		m_PauseWidgets = CreateWidget<UPauseWidgets>(GetWorld(), PauseWidgetsClass); 
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find TSubclassOf<UUserWidget>"));
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
	//Powerup Widget Enabling
	if(PowerupWidgetClass){
       m_PowerupWidget = CreateWidget<UPowerupWidget>(GetWorld(), PowerupWidgetClass);
       m_PowerupWidget->AddToViewport();
       m_PowerupWidget->SetVisibility(ESlateVisibility::Hidden);
	   if(m_PowerupWidget == NULL)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Powerup Widget not verified"));
	} 
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not find TSubclassOf<UUserWidget>"));
	}
	//Level Select Widget Enabling
	if (LevelSelectWidgetClass) {
	   m_LevelSelectWidget = CreateWidget<ULevelSelectWidget>(GetWorld(), LevelSelectWidgetClass);
	   m_LevelSelectWidget->AddToViewport();
       m_LevelSelectWidget->SetVisibility(ESlateVisibility::Hidden);
	   if(m_LevelSelectWidget == NULL)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Level Select Widget not verified"));
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
		//Pauses game
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		//Reveals mouse and enables clicking
		Mouse->bShowMouseCursor = true;
		Mouse->bEnableClickEvents = true;
		Mouse->bEnableMouseOverEvents = true;
		//Adds pause menu to screen
		UGameplayStatics::PlaySound2D(GetWorld(), pauseAudioCue);
		HideHUD(true);
		m_PauseWidgets->AddToViewport();
	}
	else {
		paused = false; 
		//Unpauses game
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		//Hides mouse and disables clicking
		Mouse->bShowMouseCursor = false;
		Mouse->bEnableClickEvents = false;
		Mouse->bEnableMouseOverEvents = false;
		//Removes the pause menu
		UGameplayStatics::PlaySound2D(GetWorld(), unPauseAudioCue);
		HideHUD(false);
		m_PauseWidgets->RemoveFromViewport();
	}
	
}

void ARunnerHUD::SetAnemonies(int anemonies) {
	anemoniesLeft = anemonies;
}

void ARunnerHUD::DecrementAnemonies() {
	anemoniesLeft--;
	if (anemoniesLeft == 0) YouWin();
}

int ARunnerHUD::getLives() {
	return m_Widgets->livesLeft;
}
int ARunnerHUD::getEnemiesLeft() {
	return m_Widgets->enemiesLeft;
}

//Win Condition Call
void ARunnerHUD::YouWin(){
	//Need to see how many maps have been beaten
	int mapsBeat = 0;
	if (UBroncoSaveGame* load = Cast<UBroncoSaveGame>(UGameplayStatics::LoadGameFromSlot("curr", 0))) {
		if (load->practiceMode) {
            Practice();
			return;
		}
		mapsBeat = load->mapsBeaten;
		load->score += m_Widgets->getScore();  //Update the score for the playthrough
		load->mapsBeaten++;
		if (UGameplayStatics::SaveGameToSlot(load, load->SaveName, 0)) { //this saves the load object
			//SHOULD display win widget, is causing an error
			class APlayerController* Mouse;
			Mouse = world->GetFirstPlayerController();
			paused = true;
			//Reveals mouse and enables clicking
			Mouse->bShowMouseCursor = true;
			Mouse->bEnableClickEvents = true;
			Mouse->bEnableMouseOverEvents = true;
			m_WinWidget->setScore(load->score); //Sets score for adding to the high scores tab
			m_WinWidget->AddToViewport(); //Displays the win screen
			m_WinWidget->PlayFadeInAnimation();
			//Pauses Game
			HideHUD(true);
			UGameplayStatics::SetGamePaused(world, true);
		}
	}
}

void ARunnerHUD::YouLose() 
{
	if (UBroncoSaveGame* load = Cast<UBroncoSaveGame>(UGameplayStatics::LoadGameFromSlot("curr", 0))) {
		if (load->practiceMode) {
            Practice();
			return;
		}
		load->score += m_Widgets->getScore();  //Update the score for the playthrough
		m_LoseWidget->setScore(load->score); //Sets score to display on the lose screen
	}
	class APlayerController* Mouse;
	Mouse = world->GetFirstPlayerController();
	paused = true;
	//Reveals mouse and enables clicking
	Mouse->bShowMouseCursor = true;
	Mouse->bEnableClickEvents = true;
	Mouse->bEnableMouseOverEvents = true;
	m_LoseWidget->AddToViewport(); //Displays the lose screen
	HideHUD(true);
	UGameplayStatics::SetGamePaused(world, true); //Pauses Game
}

void ARunnerHUD::Practice() 
{
	class APlayerController *Mouse;
	Mouse = world->GetFirstPlayerController();
	paused = true;
	Mouse->bShowMouseCursor = true;
	Mouse->bEnableClickEvents = true;
	Mouse->bEnableMouseOverEvents = true;
	m_LevelSelectWidget->SetVisibility(ESlateVisibility::Visible);
	m_LevelSelectWidget->PlayFadeInAnimation();
    UGameplayStatics::SetGamePaused(world, true);  // Pauses Game
}

void ARunnerHUD::ShowPowerupWidget(FString powerupText) 
{
    m_PowerupWidget->SetVisibility(ESlateVisibility::Visible);
	m_PowerupWidget->SetPowerupText(powerupText);	
	m_PowerupWidget->PlayFadeInAnimation(); //This animation fades the widget in and out
}

/* Rough details for adding a new widget to the HUD:
* 1. Create a .h and .cpp file for your desired widget (I.E. WinWidget.cpp and WinWidget.h)
*  		(PauseWidgets files are a good starting point, WinWidget has a more complicated menu)
* 2. #include the .h file in ARunnerHUD.h
* 3. In ARunnerHUD.cpp (this file), check that the widget is loading by adding 
* 		the "if(WidgetName)..." block.
* 4. From here on, you'll need to go into Unreal. 
*		Specifically Content/Blueprints/UI, and create a new widget
* 5. You'll need to reparent it to itself 
*		-> Open In Editor (double click) -> File -> Reparent Blueprint
* 6. Recompile and build the game
* 7. Open RunnerHUD. On the right column, find your widget and use the dropdown menu
* 		To select the proper widget
* With that, a new widget should be successfully made and implemented!
*/