// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <BroncoDrome/StageActors/PowerUp.h>
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class BRONCODROME_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DetermineRandomLocation();

	static FIntRect GetBounds();

	void SpawnObject(FVector loc, FRotator roc);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APowerUp> ActorToSpawn = APowerUp::StaticClass();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int amountOfPowerups = 6;

	static FIntRect bounds;

	FTimerHandle SpawnerTimerHandler;

	TArray<APowerUp*> powerups; //Array of PowerUps

	int actorLocationIndex = 0;

	int spacingDistance = 1500;
};
