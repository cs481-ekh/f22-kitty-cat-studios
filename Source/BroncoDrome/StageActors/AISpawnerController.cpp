// Copyright (C) KITTY CAT STUDIOS 2022. All Rights Reserved.

#include "AISpawnerController.h"
#include "AISpawner.h"
#include "AIActor.h"
#include "../Runner/Runner.h"
#include "Math/Vector.h"
#include "BroncoDrome/BroncoSaveGame.h"
#include "GameFramework/GameUserSettings.h"

// Sets default values
AAISpawnerController::AAISpawnerController() : AActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAISpawnerController::BeginPlay() {
	Super::BeginPlay();

	InitializeDifficulty(); 

	// Finds all spawn points on the map and adds them to an array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawner::StaticClass(), spawnPoints);
    for (auto &Spwner : spawnPoints) {
        numSpawnPoints++;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("NUMBER OF SPAWN POINTS: %d"), numSpawnPoints));

	GetWorldTimerManager().SetTimer(handler, this, &AAISpawnerController::Init, 12.0f, false);  // Begin spawning AI after cutscene ends
}

// Overwrites the spawner controller difficulty with the saved difficulty selection from the menu
// If for whatever reason there is no saved difficulty, difficulty will remain as set through the UProperties
void AAISpawnerController::InitializeDifficulty(){
	if (UBroncoSaveGame* load = Cast<UBroncoSaveGame>(UGameplayStatics::LoadGameFromSlot("curr", 0))) {
		difficultySetting = load->difficultySetting;
	}
}

void AAISpawnerController::Init() {
	if (initialized) return;
	initialized = true;
	// Spawn initial set of runners
	if (waveSpawning) {
		AAISpawnerController::SpawnCheck();
	} else {
		for (auto &sp: spawnPoints) {
			if (activeAI < maxAI && totalSpawned < maxRespawns) {
				AAISpawnerController::AttemptSpawn(sp);
			}
		}
	}

	// Initializes set of runners as well as gets the player runner and sets the player runner pointer
    AAISpawnerController::UpdateRunners();

	// Set spawnCheck interval. Will check if AI need to be respawned based on respawnCheckInSecs
	GetWorldTimerManager().SetTimer(SpawnTimerHandler, this, &AAISpawnerController::SpawnCheck, respawnCheckInSecs, true);	
}

void AAISpawnerController::SkipCutscene() {
	if (initialized) return;
	GetWorldTimerManager().ClearTimer(handler);
	AAISpawnerController::Init();
}

// Is called based on respawnCheckInSecs
// TODO: Account for waveSpawning
void AAISpawnerController::SpawnCheck() {
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("SPAWN CHECK, active AI: %d"), activeAI));
	if (totalSpawned >= maxRespawns) return;
	if (waveSpawning) {
        if (activeAI == 0 || waveSpawningInProgress) {
            if (!waveSpawningInProgress) {
				waveSize += waveIncrement;
				waveSpawningInProgress = true;
				GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Cyan, FString::Printf(TEXT("New wave spawning...")));
				GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Cyan, FString::Printf(TEXT("Wave Size: %d"), waveSize));
			}
			
			for (auto &sp : spawnPoints) {
				if (totalSpawned < maxRespawns && currentWaveAmountSpawned < waveSize) {
					AAISpawnerController::AttemptSpawn(sp);
					currentWaveAmountSpawned++;
				} else {
                    waveSpawningInProgress = false;
					currentWaveAmountSpawned = 0;
					break;
				}
			}
		}
	} else {
		if (activeAI >= maxAI) return;
        if (randomSpawning) { // Randomly spawns a single AI at a random spawn point
			TArray<AActor*> validSpawnPoints;
			if (!ignoreRespawnRadius) {
				// First check for valid spawn points (if there is a free spawn point where there is no runner in its radius)
				validSpawnPoints = AAISpawnerController::GetValidSpawnPoints();
			} else {
				validSpawnPoints = spawnPoints;
				numValidSpawnPoints = numSpawnPoints;
			}

			// If there's a valid spawn point, spawn a new runner at a random valid spawn point
			if (numValidSpawnPoints > 0) {
				int randSpawnPoint = FMath::RandRange(0, (numValidSpawnPoints - 1));
				int currSpawnPoint = 0;
				for (auto &sp: validSpawnPoints) {
					if (currSpawnPoint == randSpawnPoint) {
						AAISpawnerController::AttemptSpawn(sp);

						//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("SPAWNED AI AT SPAWNER: %d"), currSpawnPoint));
						return;
					} else {
						currSpawnPoint++;
					}
				}
			}
		} else {
			for (auto &sp: spawnPoints) {
				if (activeAI < maxAI && totalSpawned < maxRespawns) {
					AAISpawnerController::AttemptSpawn(sp);
				}
			}
		}
	}



}

// Returns an array of pointers to valid AISpawner objects. If there are no valid AISpawners (due to nearby runners at all points, for example) then it will return all spawn points
TArray<AActor*> AAISpawnerController::GetValidSpawnPoints() {
  TArray<AActor *> validSpawnPoints;
  numValidSpawnPoints = 0;
  for (auto &sp : spawnPoints) {
    float curDistance;
    bool validSpawn = true;
	for (int32 i = 0; i < runners.Num(); ++i) {
		if (runners.Num() != activeAI + 1) {
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Runner array size changed during iteration. VERY BAD TELL ANDREW")));
			break;
		}
		auto &runner = runners[i];
		curDistance = FVector::Dist(sp->GetActorLocation(), runner->GetActorLocation());
		if (curDistance < respawnRadius) {
			validSpawn = false;
			break;
		}
	}
    if (validSpawn) {
      numValidSpawnPoints++;
      validSpawnPoints.Add(sp);
    }
  }
  if (numValidSpawnPoints == 0) {
    numValidSpawnPoints = numSpawnPoints;
    return spawnPoints;
  }
  return validSpawnPoints;
}

// Returns number of spawn points, associated with the value returned from GetValidSpawnPoints
int AAISpawnerController::GetNumValidSpawnPoints() { 
	if (numValidSpawnPoints > 0) {
		return numValidSpawnPoints;
    } 
	return numSpawnPoints;
}

// Spawns an AI at the provided spawnPoint when called
void AAISpawnerController::AttemptSpawn(AActor* spawnPoint) {
	AActor *newAI = ((AAISpawner *)spawnPoint)->Spawn(difficultySetting);    

	if (IsValid(newAI)) {
		runners.Add(newAI);
		if (difficultySetting == "Random") {
			int randDifficulty = FMath::RandRange(0, 2);
			FName randDifficultyStr;
			switch (randDifficulty) {
				case 0:
					randDifficultyStr = FName(TEXT("Easy"));
					break;
				case 1:
					randDifficultyStr = FName(TEXT("Medium"));
					break;
				case 2:
					randDifficultyStr = FName(TEXT("Hard"));
					break;
				default:
					randDifficultyStr = FName(TEXT("Medium"));
					break;
			}
			((AAIActor*)newAI)->UpdateDifficulty(randDifficultyStr);
		} else {
			((AAIActor*)newAI)->UpdateDifficulty(difficultySetting);
		}
        activeAI++;
        totalSpawned++;
    } else {
        //spawnPoints.Remove(spawnPoint);
	}
}

// Decrements the current number of ActiveAI (this is done when an AI runner dies) so that AI will respawn
void AAISpawnerController::DecrementActiveAI(AActor* destroyedRunner) {
    activeAI--;
	runners.Remove(destroyedRunner);

    //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("ACTIVE AI DECREMENTED, NEW VAL: %d"), activeAI));
}

// Updates the list of AI runners
void AAISpawnerController::UpdateRunners() {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARunner::StaticClass(), runners);
    // numRunners should be equal to activeAI + 1 (since it includes player)
    int numRunners = 0;
	for (auto &runner : runners) {
      if (!((ARunner*)runner)->isAI) {
            playerRunner = runner;
			// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("added player runner")));
	  }
      numRunners++;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("num runners: %d"), numRunners));
}

// Returns the pointer to the player runner
AActor* AAISpawnerController::GetPlayer() {
	return playerRunner;
}

// Returns the pointer to the closest runner to a particular point
AActor* AAISpawnerController::GetClosestRunnerToPoint(FVector pt) {
	float curDistance;
    float bestDistance = 1000000;
	AActor* closestRunner = NULL;
	for (auto &runner : runners) {
		curDistance = FVector::Dist(pt, runner->GetActorLocation());
		if (curDistance < bestDistance) {
			bestDistance = curDistance;
			closestRunner = runner;
		}
	}
	return closestRunner;
}


// Called every frame, will currently spawn AI (at the spawner location) based on the respawn timer interval until max have spawned. AI currently do not respawn
void AAISpawnerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


