// Copyright (C) KITTY CAT STUDIOS 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnerController.generated.h"

UCLASS()
class BRONCODROME_API AAISpawnerController : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawnerController();
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    int getMaxAI();

    // void decrementActiveAI(); TODO future function, will decrement activeAI and increment totalDead (will be called when AI runner is destroyed)


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

    // Number of AI currently in the game
    int activeAI = 0; 
    // Store total number of dead AI
    int totalDead = 0;

    // Interval to check (in seconds) if an AI needs to be respawned
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int respawnCheckInSecs = 5;

    // Maximum amount of AI actors that are allowed to be spawned on the map. This should typically be equal to or lower than the number of AISpawner points
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int maxAIs = 5; 

    // The total number of times AI can be spawned in a given map (correlating to totalDead). This would likely only be relevant in a wave-type gamemode
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int maxRespawns = 100; 

    // Radius around the spawner that must be clear of other actors to successfully spawn at a particular spawn point
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int respawnRadius = 300; 

    // If true, at next respawn will choose a random valid spawn point, otherwise spawns equally in order
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        bool randomSpawning = true; 

    // If true, will spawn all AI actors at once in a wave when all AI are dead, similar to how AI are spawned on game initialization (this will override ignoreRespawnRadius and randomSpawning)
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        bool waveSpawning = false; 

    // If true, next spawn will be forced regardless of proximity to other runners (ignores the respawn radius)
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        bool ignoreRespawnRadius = false; 

  	// Difficulty of AI the spawners will spawn. If random, will randomly assign a difficulty on each spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetNameOptions", AllowPrivateAccess = "true")) 
        FName difficultySetting = FName(TEXT("Medium"));  

    UFUNCTION(CallInEditor)
        TArray<FString> GetNameOptions() const {
            return {TEXT("Easy"), TEXT("Medium"), TEXT("Hard"), TEXT("Random")};
        }


};