// // Copyright (C) Dromies 2021. All Rights Reserved.
/* Marking for AI team to edit*/


#include "AISpawner.h"
#include "AIActor.h"
#include "Math/Vector.h"

// Sets default values
AAISpawner::AAISpawner():AActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Used to find out how many AI there are
/*
int AAISpawner::GetAmountOfAI() {
	return amountOfAI;
}
*/

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(handler, this, &AAISpawner::AllowSpawning, 12.0f, false); // Begin spawning AI
}

// Called every frame, will currently spawn AI (at the spawner location) based on the respawn timer interval until max have spawned. AI currently do not respawn
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	respawnClock++;
	if (respawnClock > respawnTimer && canSpawn && spawnEnabled && amountOfAI < maxAI)
	{
        FVector loc = GetActorLocation();
		FRotator roc = FRotator(0, 0, 0);
        AAIActor *ai = GetWorld()->SpawnActor<AAIActor>(ActorToSpawn, loc, roc);
        aiActors.Add(ai);
        amountOfAI++;
        respawnClock = 0;
	}
}

// Enable AI spawning
void AAISpawner::AllowSpawning()
{
	canSpawn = true;
}


