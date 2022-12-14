// Copyright (C) KITTY CAT STUDIOS 2022. All Rights Reserved.


#include "AISpawner.h"
#include "AIActor.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAISpawner::AAISpawner():AActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AActor* AAISpawner::Spawn(FName difficultySetting) 
{
  if ((onlySpawnOnce && amountSpawned > 0) || !spawnEnabled) return NULL;
  FVector loc = GetActorLocation();
  FRotator rotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(),FVector(0.0f,0.0f,0.0f));
  AAIActor *ai = GetWorld()->SpawnActor<AAIActor>(ActorToSpawn, loc, rotator);
  amountSpawned++;
  spawnEnabled = false;
  return ai;
}

FVector AAISpawner::GetSpawnLocation() {
  FVector loc = GetActorLocation();
  return loc;
}


// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(handler, this, &AAISpawner::AllowSpawning, 12.0f, false); // Begin spawning AI
}

// Called every frame, will currently spawn AI (at the spawner location) based on the respawn timer interval until max have spawned. AI currently do not respawn
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*
	respawnClock++;
	if (respawnClock > respawnTimer && canSpawn && spawnEnabled && amountOfAI < maxAI)
	{
        FVector loc = GetActorLocation();
		FRotator roc = FRotator(0, 0, 0);
        AAIActor *ai = GetWorld()->SpawnActor<AAIActor>(ActorToSpawn, loc, roc);
		// Uncomment the following line at a future date when difficulty settings are implemented
        // ai->DifficultyParams.setParams(difficultySetting);
        aiActors.Add(ai);
        amountOfAI++;
        respawnClock = 0;
	}
	*/
}

// Enable AI spawning
void AAISpawner::AllowSpawning()
{
	canSpawn = true;
}


