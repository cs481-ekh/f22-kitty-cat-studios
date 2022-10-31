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
    void DecrementActiveAI(AActor* destroyedRunner);
    AActor* GetPlayer();
    AActor* GetClosestRunnerToPoint(FVector);
    TArray<AActor*> GetValidSpawnPoints();
    int GetNumValidSpawnPoints();
    void SkipCutscene();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
    void Init();
    void SpawnCheck();
    void UpdateRunners();
    void AttemptSpawn(AActor* spawnPoint);
    bool initialized = false;
    // Total number of spawn points on this map
    int numSpawnPoints = 0;
    // Number of AI currently in the game
    int activeAI = 0; 
    // Store total number of dead AI
    int totalDead = 0;
    // Store total number of AI that have been spawned over the game's lifetime
    int totalSpawned = 0;
    // Number of current available and valid spawn points
    int numValidSpawnPoints = 0;
    // List of spawn points
    TArray<AActor*> spawnPoints;
    // List of active runners
    TArray<AActor*> runners;
    // Player Runner pointer
    AActor* playerRunner;
    // Init Timer handler
    FTimerHandle handler;
    // Spawn timer handler
    FTimerHandle SpawnTimerHandler;
    // Flag if a wave spawn is in progress
    bool waveSpawningInProgress = false;
    // Counter to keep track of how many AI have spawned in the current wave
    int currentWaveAmountSpawned = 0;

    // Interval to check (in seconds) if an AI needs to be respawned
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        float respawnCheckInSecs = 5.0f;

    // Maximum amount of AI actors that are allowed to be spawned on the map. This should typically be equal to or lower than the number of AISpawner points
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int maxAI = 5; 

    // The total number of times AI can be spawned in a given map (correlating to totalDead). This would likely only be relevant in a wave-type gamemode
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int maxRespawns = 100; 

    // Radius around the spawner that must be clear of other actors to successfully spawn at a particular spawn point
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int respawnRadius = 1000; 

    // If true, at next respawn will choose a random valid spawn point, otherwise spawns equally in order
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        bool randomSpawning = true; 

    // If true, will spawn all AI actors at once in a wave when all AI are dead, similar to how AI are spawned on game initialization (this will override ignoreRespawnRadius and randomSpawning)
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        bool waveSpawning = false; 

    // If wave spawning is enabled, when a wave spawns, this is the default size. NOTE: this overrides maxAI
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int waveSize = 6; 

    // If wave spawning is enabled, this is the amount of additional AI that will be added to the waveSize per each incrementing wave.
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
        int waveIncrement = 1; 

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