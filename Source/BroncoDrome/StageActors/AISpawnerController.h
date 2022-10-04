// // Copyright (C) Dromies 2021. All Rights Reserved.
/* Marking for AI team to edit*/

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

  // How often to spawn new AI, until max AI is reached
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  int respTimer = 200;

  // Maximum amount of AI actors this spawner can spawn
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  int maxAIs = 5; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};