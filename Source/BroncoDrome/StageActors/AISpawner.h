// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BroncoDrome/StageActors/AIActor.h"
#include "GameFramework/Actor.h"
#include "AISpawner.generated.h"

UCLASS()
class BRONCODROME_API AAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawner();

	void AllowSpawning();

	int GetAmountOfAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AAIActor> ActorToSpawn = AAIActor::StaticClass();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        bool spawnEnabled = true; // Can be disabled for debugging or difficulty

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int respawnTimer= 200; // How often to spawn new AI, until max AI is reached

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int maxAI = 1; // Maximum amount of AI actors this spawner can spawn

	int amountOfAI = 0;
    int respawnClock = 0;

	FTimerHandle SpawnerTimerHandler;

	TArray<AAIActor*> aiActors;

	int actorLocationIndex = 0;

	bool canSpawn = false;

	FTimerHandle handler;

};
