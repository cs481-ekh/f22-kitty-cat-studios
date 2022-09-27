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

  double damageMod;
  double healthMod;
  double fireRateMod;

  // constructor
  FDifficultyParameters() {
    difficulty = FName(TEXT("Medium"));
    
  }

  void setParams(FName diff) {
    difficulty = diff;
    if(diff == TEXT("Easy")) {
      damageMod = healthMod = fireRateMod = 0.8;
    }
    else if(diff == TEXT("Medium")) {
      damageMod = healthMod = fireRateMod = 0.8;
    }
    else if(diff == TEXT("Hard")) {
      damageMod = healthMod = fireRateMod = 0.8;
    }
    else { // this really shouldnt happen, but we'll just go with medium
      damageMod = healthMod = fireRateMod = 0.8;
    }
  }

  /*
   *get the difficulty modifier for ai damage 
   */
  double getDifficultyDamageModifier() {return damageMod;}

  /*
 *get the difficulty modifier for ai health 
 */
  double getDifficultyHealthModifier() {return healthMod;}

  /*
  *get the difficulty modifier for ai fire rate 
  */
  double getDifficultyFireRateModifier() {return fireRateMod;}
  
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

	bool defensive = false; //0=true; 1=false; 2=false;

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
private:
	float angleBetweenTwoVectors(FVector v1, FVector v2);
	void Fire();
	void drawTargetLine(FVector location);
	void QueryLockOnEngage();
	void QueryLockOnDisengage();
	void LockOn();
        
};

