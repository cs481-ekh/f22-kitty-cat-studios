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
        bool spawnEnabled;
	
	FIntRect bounds = FIntRect(-5720, -3510, 6810, 3490);

	FTimerHandle SpawnerTimerHandler;

	TArray<AAIActor*> aiActors;

	int actorLocationIndex = 0;

	bool canSpawn = false;
    bool AISpawned = false;

	FTimerHandle handler;
};