// // Copyright (C) Dromies 2021. All Rights Reserved.
/* Marking for AI team to edit*/
/* AIActor is parent class to AIGoKart *//* AIActor is parent class to AIGoKart */

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "NavFilters/NavigationQueryFilter.h"

#include "../Runner/Runner.h"
#include "../StageActors/RunnerObserver.h"
#include "../StageActors/OrbProjectile.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIActor.generated.h"

/**
 * Struct to store and access difficulty parameters for AI actors
 */
USTRUCT(BlueprintType)
struct FDifficultyParameters {
  GENERATED_BODY()
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName difficulty;

  float damageMod;
  float healthMod;
  float fireRateMod;
  float accuracyRange;

  // constructor
  FDifficultyParameters() {
    difficulty = FName(TEXT("Medium"));
  }
  void decrementDifficulty() {
	  damageMod -= 0.1;
	  healthMod -= 0.1;
	  fireRateMod += 0.1;
	  accuracyRange += 0.01;
  }
  void setParams(FName diff) {
    difficulty = diff;
	// default HP is 100
	// default damage is 20
	// default firerate is once every 90 ticks
	// default accuracy is hard to describe, but makes most shots at close range and half or less shots at longer range
    if(diff == TEXT("Easy")) {
		healthMod = 0.8; // this will cause AI to die in 4 shots (80 hp)
		damageMod = 0.5; // this will cause AI to deal half damage (10)
        fireRateMod = 1.5; // this will cause AI to shoot 50% slower
		accuracyRange = 0.05; // relatively bad accuracy
    }
    else if(diff == TEXT("Medium")) {
		healthMod = 1.0; // this will cause AI to die in 5 shots (default)
		damageMod = 1.0; // this will cause AI to deal normal damage (20) (default)
        fireRateMod = 1.0; // this will cause AI to shoot at normal shot speed
		accuracyRange = 0.02; // decent accuracy
    }
    else if(diff == TEXT("Hard")) {
		healthMod = 1.2; // this will cause AI to die in 6 shots (120hp)
		damageMod = 1.25; // this will cause AI to deal extra damage (25)
        fireRateMod = 1.0; // this will cause AI to shoot at normal shot speed
		accuracyRange = 0.005; // good accuracy
    }
    else { // this really shouldnt happen, but we'll just go with medium
		healthMod = 1.0;
		damageMod = 1.0;
        fireRateMod = 1.0;
    }
  }
  

  /*
   *get the difficulty modifier for ai damage 
   */
  float getDifficultyDamageModifier() {return damageMod;}

  /*
 *get the difficulty modifier for ai health 
 */
  float getDifficultyHealthModifier() {return healthMod;}

  /*
  *get the difficulty modifier for ai fire rate 
  */
  float getDifficultyFireRateModifier() {return fireRateMod;}
  
  /*
  *get the accuracy range for ai 
  */
  float getDifficultyAccuracyRange() {return accuracyRange;}
};


UCLASS()
class BRONCODROME_API AAIActor : public ARunner
{
	GENERATED_BODY()

	UNavigationSystemV1* NavSys;
	FSharedConstNavQueryFilter Filter1;
	
public:	
	// Sets default values for this actor's properties
	float last_angle = 0.0;
	float curr_turn = 1.0;
	float min_angle = 165.0f;
	float max_angle = 195.0f;
	FVector last_location;
	float curr_speed = 0.0;
	float max_speed = 300.0f; //0=700.0f; 1=400.0f; 2=550.0f //was 450.0f
	float max_distance = 800.0f; //was 450.0f
	float fwd_dist = 30.0f;
	float car_distance = 1000.0f;
	float dont_care_distance = 3000.0f;
	bool reverse = false;
	class ARunner* player_runner = nullptr;
	int count = 0;
	int update_rate = 3;
	int shotCount = 0; //acts as a timer for AI shot function
	int aiId;
	int shot_rate = 90; //interval for AI shots //0=90; 1= 50; 2= 70;
	float accuracyRange; // sets accuracy of AI Runners shot

        // reaction time variables
        int frameCounter = 0;
        int reactionTime;
        enum Decisions { MoveToPlayer, MoveToSpawner };
        int lastDecision = -1;

	bool defensive = false; // fighting / fleeing state variable

        FVector lastSeenPlayerLocation; // location of the last player as seen by raycast

	AAIActor();

	void UpdateLocation(FVector point);
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Projectile to use
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AOrbProjectile> AIProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
        FDifficultyParameters DifficultyParams;

	FHitResult* Raycast(FVector to);
	FVector GetDirection();
	void MoveDecision(FVector location); //called in Tick to make move decision every 3 frames
	void MoveAwayFromPlayer(FVector player_location, FRotator player_direction);
	void MoveTowardsPlayer(FVector player_location, FRotator player_direction);
	void ShotDecision(FVector location); //called in Tick to make a shot decision every 30 frames
	void UpdateDifficulty(FName difficulty);
private:
	float angleBetweenTwoVectors(FVector v1, FVector v2);
        bool hasReduced = false;
	void Fire();
	void drawTargetLine(FVector location);
	void QueryLockOnEngage();
	void QueryLockOnDisengage();
	void LockOn();
	FVector GetAccuracyVector();
        
};

