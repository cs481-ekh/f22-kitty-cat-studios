// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "AISpawner.h"

// Sets default values
AAISpawner::AAISpawner():AActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (canSpawn)
	{
		for (amountOfAI = 0; amountOfAI < maxAI; amountOfAI++) {
			//FVector loc = FVector(FMath::RandRange(bounds.Min.X, bounds.Max.X), FMath::RandRange(bounds.Min.Y, bounds.Max.Y), -100);
			FVector loc = FVector(FMath::RandRange(bounds.Min.X, bounds.Max.X), FMath::RandRange(bounds.Min.Y, bounds.Max.Y), 400);

			FRotator roc = FRotator(0, 0, 0);
			aiActors.Add(GetWorld()->SpawnActor<AAIActor>(ActorToSpawn, loc, roc));
		}
	}

	GetWorldTimerManager().SetTimer(handler, this, &AAISpawner::AllowSpawning, 12.0f, false);
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canSpawn)
	{
		if (amountOfAI < maxAI) {
			for (amountOfAI; amountOfAI < maxAI; amountOfAI++) {
				FVector loc = FVector(FMath::RandRange(bounds.Min.X, bounds.Max.X), FMath::RandRange(bounds.Min.Y, bounds.Max.Y), 400);
				FRotator roc = FRotator(0, 0, 0);
				aiActors.Add(GetWorld()->SpawnActor<AAIActor>(ActorToSpawn, loc, roc));
			}
		}
	}
}

void AAISpawner::AllowSpawning()
{
	canSpawn = true;
}


