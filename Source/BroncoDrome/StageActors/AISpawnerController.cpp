// Copyright (C) KITTY CAT STUDIOS 2022. All Rights Reserved.


#include "AISpawner.h"
#include "../Runner/Runner.h"
#include "Math/Vector.h"

// Sets default values
AAISpawnerController::AAISpawnerController() : AActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAISpawnerController::BeginPlay() {
	Super::BeginPlay();

	// Finds all spawn points on the map and adds them to an array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawner::StaticClass(), spawnPoints);
    for (auto &Spwner : spawnPoints) {
        numSpawnPoints++;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("NUMBER OF SPAWN POINTS: %d"), numSpawnPoints));

	GetWorldTimerManager().SetTimer(handler, this, &AAISpawnerController::Init, 12.0f, false);  // Begin spawning AI after cutscene ends
}

void AAISpawnerController::Init() {
	// Initially populate all spawners with an AI, if below maxAI threshold, on game start
    for (auto &sp: spawnPoints) {
		if (activeAI < maxAI) {
            AAISpawnerController::AttemptSpawn(sp);
		}
	}
    AAISpawnerController::UpdateRunners();

	// Set spawnCheck interval. Will check if AI need to be respawned based on respawnCheckInSecs
	GetWorldTimerManager().SetTimer(SpawnTimerHandler, this, &AAISpawnerController::SpawnCheck, respawnCheckInSecs, true);	
}

// Is called based on respawnCheckInSecs
// TODO: Account for maxRespawns, waveSpawning
void AAISpawnerController::SpawnCheck() {
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("SPAWN CHECK, active AI: %d"), activeAI));
	if (activeAI >= maxAI) return;
  AAISpawnerController::UpdateRunners();
	if (randomSpawning) { // Randomly spawns a single AI at a random spawn point
		int numValidSpawnPoints = numSpawnPoints;
		TArray<AActor*> validSpawnPoints;
      if (!ignoreRespawnRadius) {
        // First check for valid spawn points (if there is a free spawn point where there is no runner in its radius)
        numValidSpawnPoints = 0;
        for (auto &sp: spawnPoints) {
           float curDistance;
           bool validSpawn = true;
           for (auto &runner : runners) {
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
			  //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Valid spawn points: %d"), numValidSpawnPoints));
      } else {
           validSpawnPoints = spawnPoints;
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
			if (activeAI < maxAI) {
                AAISpawnerController::AttemptSpawn(sp);
			}
		}
	}

}

// Spawns an AI at the provided spawnPoint when called
void AAISpawnerController::AttemptSpawn(AActor* spawnPoint) {
    ((AAISpawner*)spawnPoint)->Spawn("Medium");
    activeAI++;
	totalSpawned++;
}

// Decrements the current number of ActiveAI (this is done when an AI runner dies) so that AI will respawn
void AAISpawnerController::DecrementActiveAI() {
  activeAI--;
  //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("ACTIVE AI DECREMENTED, NEW VAL: %d"), activeAI));
}

// Updates the list of AI runners
void AAISpawnerController::UpdateRunners() {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARunner::StaticClass(), runners);
    /* numRunners should be equal to activeAI + 1 (since it includes player)
    int numRunners = 0;
	for (auto &runner : runners) {
          numRunners++;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("num runners: %d"), numRunners));*/
}

// Called every frame, will currently spawn AI (at the spawner location) based on the respawn timer interval until max have spawned. AI currently do not respawn
void AAISpawnerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


