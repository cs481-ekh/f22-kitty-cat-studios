// // Copyright (C) Gregg 2022. All Rights Reserved.


#include "BroncoSaveGame.h"


/* 
* This class represents a persistent save file that can be saved an loaded
* no matter what level the player is on
*/
UBroncoSaveGame::UBroncoSaveGame()
{
	//initialize variables
	score = 0; //Represents the overall score
	mapsBeaten = 0; //Represents how many maps have been beaten
	SaveName = TEXT("curr"); //We set the save name here because there will only be 1 save game to keep track of the current playthrough

	runnerSelection = FName(TEXT("balanced"));
}