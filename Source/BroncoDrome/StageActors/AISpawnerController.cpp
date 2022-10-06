// Copyright (C) KITTY CAT STUDIOS 2022. All Rights Reserved.


#include "AISpawner.h"
#include "AIActor.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("NUMBER OF SPAWN POINTS: %d"), numSpawnPoints));

	GetWorldTimerManager().SetTimer(handler, this, &AAISpawnerController::Init, 12.0f, false);  // Begin spawning AI after cutscene ends
}

void AAISpawnerController::Init() {
	// Initially populate all spawners with an AI, if below maxAI threshold, on game start
    for (auto &sp: spawnPoints) {
		if (activeAI < maxAI) {
            AAISpawnerController::AttemptSpawn(sp);
		}
	}

	// Set spawnCheck interval. Will check if AI need to be respawned based on respawnCheckInSecs
	GetWorldTimerManager().SetTimer(SpawnTimerHandler, this, &AAISpawnerController::SpawnCheck, respawnCheckInSecs, true);	
}

// Is called based on respawnCheckInSecs
// TODO: Account for respawnRadius, maxRespawns, waveSpawning, dead AI
void AAISpawnerController::SpawnCheck() {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("SPAWN CHECK, active AI: %d"), activeAI));
	if (activeAI >= maxAI) return;
	if (randomSpawning) { // Randomly spawns a single AI at a random spawn point
		int randSpawnPoint = FMath::RandRange(0, (numSpawnPoints - 1));
        int currSpawnPoint = 0;
		for (auto &sp: spawnPoints) {
            if (currSpawnPoint == randSpawnPoint) {
			    AAISpawnerController::AttemptSpawn(sp);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("SPAWNED AI AT SPAWNER: %d"), currSpawnPoint));
				return;
            } else {
			    currSpawnPoint++;
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

// Called every frame, will currently spawn AI (at the spawner location) based on the respawn timer interval until max have spawned. AI currently do not respawn
void AAISpawnerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


