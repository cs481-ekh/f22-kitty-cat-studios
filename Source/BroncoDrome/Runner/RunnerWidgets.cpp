// // Copyright (C) Dromies 2021. All Rights Reserved.

#include "RunnerWidgets.h"
#include "BroncoDrome/BroncoSaveGame.h"
#include "Kismet/GameplayStatics.h"

void URunnerWidgets::NativePreConstruct()
{
	Super::NativePreConstruct();


}

void URunnerWidgets::NativeConstruct()
{
	Super::NativeConstruct();

	playerScore = 0; 

	// Check if survival mode is active (to update HUD)
	if (UBroncoSaveGame* load = Cast<UBroncoSaveGame>(UGameplayStatics::LoadGameFromSlot("curr", 0))) {
		if (load->gamemodeSelection == TEXT("Survival")) {
			survivalMode = true;
			currentWave = 1;
		}
		if (load->difficultySetting == TEXT("Easy")) {
			difficultyScoreModifier = 0.5f;
		} else if (load->difficultySetting == TEXT("Medium")) {
			difficultyScoreModifier = 1.f;
		} else {
			difficultyScoreModifier = 1.5f;
		}
	}
}

void URunnerWidgets::increaseScoreVar(int score) {
	playerScore += score;
}

void URunnerWidgets::SetDead(bool newDead) {
	dead = newDead;
}

void URunnerWidgets::SetHealth(int newHealth) {
	healthFloat = newHealth / 100.0; 
}

void URunnerWidgets::SetTimeLeft(int newTime) {
	timeLeft = newTime;
}

void URunnerWidgets::SetSpeed(float newSpeed) {
	speed = newSpeed; 
}

void URunnerWidgets::SetGameTimeRemaining(int newTime){
	gameTimeRemaining = newTime;
}

void URunnerWidgets::IncrementCurrentWave() {
	currentWave++;
}

void URunnerWidgets::DecrementLivesLeft() {
	livesLeft--;
}

void URunnerWidgets::SetEnemiesLeft(int newAmount) {
	enemiesLeft = newAmount;
}

void URunnerWidgets::DecrementEnemiesLeft(void) {
	enemiesLeft--;
}

//Return the player score on a win
int URunnerWidgets::getScore() {
	if (survivalMode) {
		playerScore *= (difficultyScoreModifier * 10.f); // Give boost since player always has no time and/or lives remaining on survival loss
	}
	else {
		playerScore += ((gameTimeRemaining*10) + (livesLeft*100)) * difficultyScoreModifier; // Factor in the time and lives remaining and difficulty
	}
	
	return playerScore;
}

//Calculate the player score on a loss (specifically, ignore game time remaining)
int URunnerWidgets::getScoreLoss() {
	if (survivalMode) {
		playerScore *= (difficultyScoreModifier * 10.f); // Give boost since player always has no time and/or lives remaining on survival loss
	}
	else {
		playerScore *= difficultyScoreModifier; // Factor in the difficulty modifier
	}
	
	return playerScore;
}

//Return the current wave the player has reached (in survival mode)
int URunnerWidgets::getWave() {
	return currentWave;
}

bool URunnerWidgets::getAutoTargetToggled() {
	return autoTarget;
}

void URunnerWidgets::setAutoTarget(bool val) {
	autoTarget = val;
}