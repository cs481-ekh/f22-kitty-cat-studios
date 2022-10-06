// Copyright (C) KITTY CAT STUDIOS 2022. All Rights Reserved.


#include "AISpawner.h"
#include "AIActor.h"

// Sets default values
AAISpawnerController::AAISpawnerController() : AActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

int AAISpawnerController::getMaxAI() { return maxAIs; }

// Called when the game starts or when spawned
void AAISpawnerController::BeginPlay() {
	Super::BeginPlay();

	// Finds all spawn points on the map and adds them to an array
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawner::StaticClass(), FoundActors);

	// For debugging purposes and example of access, below iterates through spawners to show how to access spawner functions
	int numSpawners = 0;
    for (auto &Spwner : FoundActors) {
        numSpawners++;
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("SPAWN POINT %d MAX AI: %d"), numSpawners, ((AAISpawner*)Spwner)->GetAmountOfAI()));
	}

	// Prints number of spawners for debugging purposes
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("NUMBER OF SPAWN POINTS: %d"), numSpawners));
}

// Called every frame, will currently spawn AI (at the spawner location) based on the respawn timer interval until max have spawned. AI currently do not respawn
void AAISpawnerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


