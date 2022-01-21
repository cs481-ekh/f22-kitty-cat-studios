// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "Spawner.h"
#include <cmath>

FIntRect ASpawner::bounds = FIntRect(-5720, -3510, 6810, 3490);

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	for(int x = 0; x < amountOfPowerups; x++)
		powerups.Add(GetWorld()->SpawnActor<APowerUp>(ActorToSpawn, FVector(0,0,-500), FRotator(0,0,0)));

	GetWorldTimerManager().SetTimer(SpawnerTimerHandler, this, &ASpawner::DetermineRandomLocation, 4.0f, true);
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::DetermineRandomLocation()
{
	FVector loc = FVector(FMath::RandRange(bounds.Min.X,bounds.Max.X), FMath::RandRange(bounds.Min.Y,bounds.Max.Y), -70);

	for (int i = 0; i < powerups.Num(); i++)
	{
		if (actorLocationIndex == i)
			continue;
		FVector otherloc = powerups[i]->GetActorLocation();

		//Change location and try again if they are too close.
		if (sqrt(pow(abs(loc.X - otherloc.X), 2) + pow(abs(loc.Y - otherloc.Y), 2)) <= spacingDistance)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Powerup too close to another powerup, relocating..."), *GetDebugName(this)));
			loc = FVector(FMath::RandRange(bounds.Min.X, bounds.Max.X), FMath::RandRange(bounds.Min.Y, bounds.Max.Y), -70);
			i = 0;
		}
	}

	FRotator roc = FRotator(0, 0, 0);

	SpawnObject(loc, roc);
}

FIntRect ASpawner::GetBounds()
{
	return bounds;
}

void ASpawner::SpawnObject(FVector loc, FRotator roc)
{
	powerups[actorLocationIndex]->UpdateLocation(loc);

	actorLocationIndex++;
	if (actorLocationIndex == powerups.Num())
		actorLocationIndex = 0;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("A powerup has been spawned."), *GetDebugName(this)));

}

