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

//Return the current player score
int URunnerWidgets::getScore() {
	return playerScore;
}


