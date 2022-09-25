// // Copyright (C) Dromies 2021. All Rights Reserved.


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

	if (canSpawn) // this function is currently not called as canSpawn isn't set till true till after the game startts
	{
          /*
		for (amountOfAI = 0; amountOfAI < maxAI; amountOfAI++) {
			//FVector loc = FVector(FMath::RandRange(bounds.Min.X, bounds.Max.X), FMath::RandRange(bounds.Min.Y, bounds.Max.Y), -100);
            FVector loc = GetActorLocation();
                        GEngine->AddOnScreenDebugMessage(
                            -1, 5.f, FColor::Red,
                            FString::Printf(TEXT("%f"), GetActorLocation().X));
			FRotator roc = FRotator(0, 0, 0);
			AAIActor *ai = GetWorld()->SpawnActor<AAIActor>(ActorToSpawn, loc, roc);
			if (amountOfAI == 0) { //Defensive unit
				ai->max_speed = 700.0f;
				ai->shot_rate = 90;
				ai->defensive = true;
			}
			if (amountOfAI == 1) { //Offensive unit
				ai->max_speed = 400.0f;
				ai->shot_rate = 50;
				ai->defensive = false;
			}
			if (amountOfAI == 2) { //Offensive unit
				ai->max_speed = 550.0f;
				ai->shot_rate = 70;
				ai->defensive = false;
			}
			 

			aiActors.Add(ai);
		}*/
		//Cast<ARunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetEnemiesLeft(maxAI);
	}

	GetWorldTimerManager().SetTimer(handler, this, &AAISpawner::AllowSpawning, 12.0f, false);
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (canSpawn && spawnEnabled && !AISpawned)
	{
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SPAWNING!")));
        FVector loc = GetActorLocation();
		FRotator roc = FRotator(0, 0, 0);
		aiActors.Add(GetWorld()->SpawnActor<AAIActor>(ActorToSpawn, loc, roc));
        int len = sizeof(aiActors) / sizeof(aiActors[0]);
                //GEngine->AddOnScreenDebugMessage(
                //    -1, 5.f, FColor::Red, FString::Printf(TEXT("length: %d"), len));
        AISpawned = true;
	}
}

void AAISpawner::AllowSpawning()
{
	canSpawn = true;
}


