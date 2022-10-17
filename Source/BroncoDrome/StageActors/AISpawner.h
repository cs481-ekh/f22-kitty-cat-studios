// Copyright (C) KITTY CAT STUDIOS 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BroncoDrome/StageActors/AIActor.h"
#include "BroncoDrome/StageActors/AISpawnerController.h"
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

	AActor* Spawn(FName difficultySetting);

	FVector GetSpawnLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AAIActor> ActorToSpawn = AAIActor::StaticClass();

	// Can be disabled for debugging or difficulty
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true")) 
        bool spawnEnabled = true; 

	// Can be disabled for debugging or difficulty
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true")) 
        bool onlySpawnOnce = false; 

	int amountSpawned = 0;
    int respawnClock = 0;

	bool canSpawn = false;

	FTimerHandle handler;
};