// // Copyright (C) Dromies 2021. All Rights Reserved.

#include "RunnerWidgets.h"

void URunnerWidgets::NativePreConstruct()
{
	Super::NativePreConstruct();


}

void URunnerWidgets::NativeConstruct()
{
	Super::NativeConstruct();

	playerScore = 0; 
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

void URunnerWidgets::DecrementLivesLeft() {
	livesLeft--;
}

void URunnerWidgets::SetEnemiesLeft(int newAmount) {
	enemiesLeft = newAmount;
}

void URunnerWidgets::DecrementEnemiesLeft(void) {
	enemiesLeft--;
}

//Return the current player score
int URunnerWidgets::getScore() {
	playerScore += (gameTimeRemaining*10) + (livesLeft*100); // Factor in the time and lives remaining
	return playerScore;
}

bool URunnerWidgets::getAutoTargetToggled() {
	return autoTarget;
}

void URunnerWidgets::setAutoTarget(bool val) {
	autoTarget = val;
}