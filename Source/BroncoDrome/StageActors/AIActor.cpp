// // Copyright (C) Dromies 2021. All Rights Reserved.
/* Marking for AI team to edit*/


#include "AIActor.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"
#include "AISpawner.h"
#include "GameFramework/Character.h"
#include "PowerUp.h"
#include "PowerUp/PowerUpMaster.h"

#include "Math/RandomStream.h"


// Sets default values
AAIActor::AAIActor() : ARunner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isAI = true;
	// ARunner::aiId = AISpawner::GetAmountOfAI();
	//Mover = (UChaosWheeledVehicleMovementComponent*)GetMovementComponent();
	//Mover->SetThrottleInput(1.0f);
	//Mover->bReverseAsBrake = true;

}

void AAIActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->ClearActionBindings();
	PlayerInputComponent->ClearBindingValues();
}

// Called when the game starts or when spawned
void AAIActor::BeginPlay()
{
	Super::BeginPlay();
	last_location = GetActorLocation();
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
        currPos = this->GetActorLocation();
        prevPos = this->GetActorLocation();
  
    DifficultyParams = FDifficultyParameters();
	UpdateDifficulty(FName(TEXT("Medium"))); // Set default difficulty
    reactionTime = 6;
    pos_counter = 0;
    pos_buffer = 0;

	if (UBroncoSaveGame* load = Cast<UBroncoSaveGame>(UGameplayStatics::LoadGameFromSlot("curr", 0))) {
		if (load->gamemodeSelection == TEXT("Survival")) {
			friendlyFire = false;
		}
		else {
			friendlyFire = true;
		}
	}
}

// Updates relevant difficulty settings for this runner
void AAIActor::UpdateDifficulty(FName difficulty) {
	DifficultyParams.setParams(difficulty);

	accuracyRange = DifficultyParams.getDifficultyAccuracyRange();
	playerDamage = 20 * DifficultyParams.getDifficultyDamageModifier();
	health = 100 * DifficultyParams.getDifficultyHealthModifier();
	shot_rate = 90 * DifficultyParams.getDifficultyFireRateModifier();
}

// Returns a vector to modify the shot angle based on difficulty
FVector AAIActor::GetAccuracyVector() {
	float accuracyRangeX, accuracyRangeY, accuracyRangeZ;

	accuracyRangeX = FMath::FRandRange(-accuracyRange, accuracyRange);
	accuracyRangeY = FMath::FRandRange(-accuracyRange, accuracyRange);
	accuracyRangeZ = FMath::FRandRange(-accuracyRange, accuracyRange);
	
	FVector accVec = FVector(accuracyRangeX, accuracyRangeY, accuracyRangeZ);

	return accVec;
}

// Called every frame
void AAIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  
        currPos = this->GetActorLocation();

        if(pos_buffer > 0 && pos_buffer < 60) {
          ThrottleInput(-1.0f);
          pos_buffer++;
          if(pos_buffer == 59)
            pos_buffer = 0;
        //  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("STUCK! REVERSE"), *GetDebugName(this)));
          return;
        }
        if(currPos.Equals(prevPos,8)) {
          pos_counter++;
          if(pos_counter > 50) {
            ThrottleInput(-1.0f);
            pos_buffer = 1;
         //   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("STUCK! REVERSE"), *GetDebugName(this)));
            return;
          }
          
        }
        else {
          pos_counter = 0;
        }
  
  
        
        
        
        frameCounter++;
	if (IsGrounded()) {
		// Mover->SetThrottleInput(1.0f);
		auto location = GetActorLocation(); //Obtains the (x,y,z) vector loction of the AI
		const float speed = Mover->GetForwardSpeedMPH();
		if (speed < maxSpeed && !reverse) { // Max speed is defined in runner.cpp
			ThrottleInput(1.0f);
		}
		else {
			ThrottleInput(-1.0f);
		}
		drawTargetLine(location);
		MoveDecision(location);
		//ShotDecision(location);
		last_location = location;
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Moving"), *GetDebugName(this)));
	}
	else {
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("In Air "), *GetDebugName(this)));
		m_TorqueInput.Y = 0;
		m_TorqueInput.X = 0;
		m_TorqueInput.Z = 0;
	}
	// Mover->SetSteeringInput(in);

	// TODO: Set defensive variable according to runner health
	// TODO: Remove if already handled in the Movement function
        // i dont think that we need this.  move towards player will be called in move decision higher up in the code
	//if(!defensive)
         // MoveTowardsPlayer(GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));

        //player_runner = ARunnerObserver::GetPlayer(*this, 38000.f, 180.f, true);


        if (player_runner != NULL && !player_runner->isAI) { // runner may not have seen them, but when they do, difficulty will be updated
          if(player_runner->lives == 1 && !hasReduced) {
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TESTY DESTY"), *GetDebugName(this)));
            hasReduced = true;
            DifficultyParams.decrementDifficulty();
            
          }
          
        }
        prevPos = this->GetActorLocation();
}

FHitResult* AAIActor::Raycast(FVector to)
{
	FHitResult outHit;
	FHitResult* oHit;
	const FCollisionQueryParams collisionParams(FName(TEXT("TestCast")), false, this);
	// collisionParams.AddIgnoredActor(*this);

	FVector start = GetActorLocation();
	bool hit = GetWorld()->LineTraceSingleByChannel(outHit, start, start + to, ECC_WorldDynamic, collisionParams);
	if (hit) {
		oHit = &outHit;
		return oHit;
	}
	else {
		return nullptr;
	}
}

FVector AAIActor::GetDirection() {

	FVector result = FVector(0.0f, 0.0f, 0.0f);
	float closest = 9999999.0f;

	FHitResult* forward = Raycast(GetActorForwardVector() * max_distance);
	if (forward) {
		if (forward->Distance + fwd_dist < closest) {
			closest = forward->Distance + fwd_dist;
			result = FVector::ForwardVector;
		}
	}

	FHitResult* left = Raycast(-GetActorRightVector() * max_distance);
	if (left) {
		if (left->Distance < closest) {
			closest = left->Distance;
			result = FVector::LeftVector;
		}
	}

	FHitResult* right = Raycast(GetActorRightVector() * max_distance);
	if (right) {
		if (right->Distance < closest) {
			closest = right->Distance;
			result = FVector::RightVector;
		}
	}

	FHitResult* fleft = Raycast((-GetActorRightVector() + GetActorForwardVector()) * max_distance);
	if (fleft) {
		if (fleft->Distance < closest) {
			closest = fleft->Distance;
			result = (FVector::LeftVector + FVector::ForwardVector);
		}
	}

	FHitResult* fright = Raycast((GetActorRightVector() + GetActorForwardVector()) * max_distance);
	if (fright) {
		if (fright->Distance < closest) {
			closest = fright->Distance;
			result = (FVector::RightVector + FVector::ForwardVector);
		}
	}

	//FHitResult* backward = Raycast(-GetActorForwardVector() * max_distance);
	//if (backward) {
	//	if (backward->Distance + fwd_dist < closest) {
	//		closest = backward->Distance + fwd_dist;
	//		result = FVector::BackwardVector;
	//	}
	//}
	//I want to leave out this vector for some testing

	FHitResult* bright = Raycast((GetActorRightVector() + (-GetActorForwardVector())) * max_distance);
	if (bright) {
		if (bright->Distance < closest) {
			closest = bright->Distance;
			result = (FVector::RightVector + FVector::BackwardVector);
		}
	}

	FHitResult* bleft = Raycast(((-GetActorRightVector()) + (-GetActorForwardVector())) * max_distance);
	if (bleft) {
		if (bleft->Distance < closest) {
			closest = bleft->Distance;
			result = (FVector::LeftVector + FVector::BackwardVector);
		}
	}
	return result;
}


void AAIActor::MoveDecision(FVector location) {
  if (count != update_rate) {
    count++;
    return;
  }
  count = 0;

  // auto player_location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
  auto closest_runner = ARunnerObserver::GetClosestRunner(*this);
  auto closest_powerup = ARunnerObserver::GetClosestPowerup(*this);
  if (!closest_runner->isAI) {
    player_runner = closest_runner;
  }
  
  // auto closest_runner = ARunnerObserver::GetPlayer(*this);
  auto player_location = closest_runner->GetActorLocation();
  auto powerup_location = closest_powerup;
  auto player_direction = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();

  auto curr_location = GetActorLocation();
  auto curr_direction = GetActorRotation();

  auto forward_vec = GetActorForwardVector();
  // ActorGetDistanceToCollision()
  // ActorLineTraceSingle()
  auto dir = GetDirection();
  reverse = dir.Equals(FVector::ForwardVector);

  //this vector may be coming from the center of objects, which is why runners used to continuously run into objects
    if (!dir.Equals(FVector(0.0f, 0.0f, 0.0f))) {
      if (dir.Equals(FVector::ForwardVector)) {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("forward reverse"), *GetDebugName(this)));
        Mover->SetSteeringInput(-0.6f);
        ThrottleInput(-1.0f);
      }
      else if (dir.Equals(FVector::LeftVector)) {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("left forward"), *GetDebugName(this)));
        Mover->SetSteeringInput(0.6f);
        ThrottleInput(1.0f);
      }
      else if (dir.Equals(FVector::RightVector)) {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("right forward"), *GetDebugName(this)));
        Mover->SetSteeringInput(-0.6f);
        ThrottleInput(1.0f);
      }
      else if (dir.Equals((FVector::LeftVector + FVector::ForwardVector))) {
        //Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("left forward reverse"), *GetDebugName(this)));
        Mover->SetSteeringInput(0.6f);
        ThrottleInput(-1.0f);
      }
      else if (dir.Equals((FVector::RightVector + FVector::ForwardVector))) {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("right forward reverse"), *GetDebugName(this)));
        Mover->SetSteeringInput(-0.6f);
        ThrottleInput(-1.0f);
      }
    }
  
   if (frameCounter >= reactionTime) {
            
            // there are no nearby runners, should try to move towards a nearby power up
            if (player_runner && ARunnerObserver::GetRunnerDistance(*this, *player_runner) > 5000) {
              //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("No nearby runners"), *GetDebugName(this)));
              MoveTowardsPowerUp(powerup_location->GetActorLocation());
              lastDecision = 1;
              reactionTime = FMath::RandRange(4,12);
              frameCounter = 0;
            }
        
        
            else if (defensive) {
              if(frameCounter >= reactionTime || lastDecision == 0) {
               //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("defensive: move away"), *GetDebugName(this)));
                MoveAwayFromPlayer(player_location, player_direction);
                ThrottleInput(-1.0f);
                lastDecision = 0;
                reactionTime = FMath::RandRange(4,12);
                frameCounter = 0;

              }
            }
            else {
              if(frameCounter >= reactionTime || lastDecision == 0) {
               // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("move towards player"), *GetDebugName(this)));
                MoveTowardsPlayer(player_location, player_direction);
                ThrottleInput(1.0f);
                lastDecision = 0;
                reactionTime = FMath::RandRange(4,12);
                frameCounter = 0;
              }
            }
          }

}

//=======FOR AI SHOOTING

void AAIActor::QueryLockOnEngage() {
	// Check to see if timer is valid
	if (!m_LockOnQueryTimer.IsValid() || !player_runner)
		return;
	// Query to see if other runner is visible
	if (ARunnerObserver::IsRunnerVisible(*this, *player_runner, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST)) {
		// Loop another engagement query timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	}

	else
		// Start disengage timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnDisengage, LOCK_ON_NON_VISIBLE_TIME, false);
}

void AAIActor::QueryLockOnDisengage() {
	// Check to see if timer is valid
	if (!m_LockOnQueryTimer.IsValid() || !player_runner)
		return;
	// Query to see if other runner is visible
	if (ARunnerObserver::IsRunnerVisible(*this, *player_runner, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST))
		// Return to engagemenet query timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	else
		player_runner = nullptr;

}
void AAIActor::LockOn() {
	// Currently locked on?
	if (!player_runner) {
		// Target closest visible Runner (may return null)
		player_runner = ARunnerObserver::GetClosestRunner(*this, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST);
		// If found target, start query tick
		if (player_runner)
			GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	}
	else {
		// Clear currently locked-on Runner
		player_runner = nullptr;
		// Stop any timers that may be running
		m_LockOnQueryTimer.Invalidate();
	}
}
void AAIActor::drawTargetLine(FVector location) {
	
	player_runner = ARunnerObserver::GetPlayer(*this, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, true);
	ARunner* aRunner = ARunnerObserver::GetClosestRunner(*this, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST);
	ARunner* setTargetRunner;

	if (player_runner) {

		setTargetRunner = player_runner;

		ARunner::AimBlaster(setTargetRunner->GetActorLocation(), GetWorld()->GetDeltaSeconds());
		LockOn();

		if (shotCount < shot_rate) {  // shot timer (currently set to
											  // one shot every 30 frames
			shotCount++;
			return;
		}
		shotCount = 0;  // Reset timer
		// drawTargetLine(location);
		Fire();

	}
	else if (aRunner && friendlyFire) {

		setTargetRunner = aRunner;

		ARunner::AimBlaster(setTargetRunner->GetActorLocation(), GetWorld()->GetDeltaSeconds());
		LockOn();

		if (shotCount < shot_rate) {  // shot timer (currently set to
											  // one shot every 30 frames
			shotCount++;
			return;
		}
		shotCount = 0;  // Reset timer
		// drawTargetLine(location);
		Fire();
	}
}

void AAIActor::ShotDecision(FVector location) {
	/*if (shotCount < shot_rate) { 
		shotCount++;
		return;
	}
	shotCount = 0; //Reset timer
	 drawTargetLine(location);
	Fire();*/
}

void AAIActor::Fire() {

	if (AIProjectileClass) {
		auto World = GetWorld();
		if (!World) return;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		const auto rot = BlasterBase->GetComponentRotation();
		const auto loc = BlasterBase->GetComponentLocation() + 150.0 * (rot.Vector());	//150 is to account for the length of the barrel
		AOrbProjectile* Projectile = World->SpawnActor<AOrbProjectile>(AIProjectileClass, loc, rot, SpawnParams);
		if (Projectile) {
			PlaySound(laserAudioCue);
			Projectile->FireOrbInDirection(rot.Vector()+GetAccuracyVector(), this);
			Projectile->shotDamage = playerDamage; //Set damage of shot to the runners damage
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Projectile init error"), *GetDebugName(this)));
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ProjectileClass error"), *GetDebugName(this)));
	}
}
//=======END AI SHOOTING
void AAIActor::MoveAwayFromPlayer(FVector player_location, FRotator player_direction) {
	auto curr_location = GetActorLocation();
	auto curr_direction = GetActorRotation();
	lastSeenPlayerLocation = player_location;


	// FOR RANDOM MOVES IF WE WANT
	//FNavLocation ResultLocation = FNavLocation();
	//auto radius = 500.0;
	//NavSys->GetRandomReachablePointInRadius(player_location, radius, ResultLocation);

	auto turn_rotation = UKismetMathLibrary::FindLookAtRotation(player_location, curr_location);
	auto turn_angle_manhattan = turn_rotation.GetManhattanDistance(curr_direction);
	//if ((last_angle == turn_angle_manhattan && turn_angle_manhattan > min_angle) || (turn_angle_manhattan == last_angle && turn_angle_manhattan < max_angle)) {
	//	curr_turn = -curr_turn;
	//	last_angle = turn_angle_manhattan;
	//	Mover->SetSteeringInput(curr_turn);
	//}
	//else {
	//	if (turn_angle_manhattan < min_angle || turn_angle_manhattan > max_angle) {
	//		Mover->SetSteeringInput(0.0f);
	//	}
	//	else {
	//		Mover->SetSteeringInput(curr_turn);
	//	}
	//}
	if (turn_angle_manhattan < max_angle && turn_angle_manhattan > min_angle) {
		Mover->SetSteeringInput(turn_angle_manhattan);
	}
	else if (turn_angle_manhattan > max_angle) {
		Mover->SetSteeringInput(max_angle);
	}
	else {
		Mover->SetSteeringInput(min_angle);
	}
}

void AAIActor::MoveTowardsPlayer(FVector player_location, FRotator player_direction)
{
	auto curr_location = GetActorLocation();
	auto curr_direction = GetActorRotation();
	lastSeenPlayerLocation = player_location;


	auto turn_rotation = UKismetMathLibrary::FindLookAtRotation(player_location, curr_location);
	auto turn_angle_manhattan = turn_rotation.GetManhattanDistance(curr_direction);
     //  GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("Manhattan: %lf"), turn_angle_manhattan));
  
	 if (turn_angle_manhattan < max_angle && turn_angle_manhattan > min_angle) {
	   
	   Mover->SetSteeringInput(-turn_angle_manhattan);
	//  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BETWEEN MIN AND MAX"), *GetDebugName(this)));

	}
	else if (turn_angle_manhattan > max_angle) {
		Mover->SetSteeringInput(max_angle);
	//  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("GREATER THAN MAX"), *GetDebugName(this)));

	}
	else {
		Mover->SetSteeringInput(min_angle);
	 // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LESS THAN MAX"), *GetDebugName(this)));

	}
}

void AAIActor::MoveTowardsPowerUp(FVector powerup_location)
{
  auto curr_location = GetActorLocation();
  auto curr_direction = GetActorRotation();
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("move towards powerup"), *GetDebugName(this)));



  auto turn_rotation = UKismetMathLibrary::FindLookAtRotation(powerup_location, curr_location);
  auto turn_angle_manhattan = turn_rotation.GetManhattanDistance(curr_direction);

  if (FVector::Dist(this->GetActorLocation(), powerup_location) < 300.f) {
   // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("moving straight"), *GetDebugName(this)));

  }
  else if (turn_angle_manhattan < max_angle && turn_angle_manhattan > min_angle) {
    Mover->SetSteeringInput(-turn_angle_manhattan);
  }
  else if (turn_angle_manhattan > max_angle) {
    Mover->SetSteeringInput(max_angle);
  }
  else {
    Mover->SetSteeringInput(min_angle);
  }
}

void AAIActor::UpdateLocation(FVector point)
{
	//SetActorLocation(point, false, 0, ETeleportType::None);

	//GetActorLocation().Set(point.X, point.Y, point.Z);

	//SetActorHiddenInGame(false);
	//SetActorTickEnabled(true);
	// SetActorEnableCollision(true);
}

float AAIActor::angleBetweenTwoVectors(FVector v1, FVector v2) {
	v1 = v1.GetSafeNormal();
	v2 = v2.GetSafeNormal();

	float holdDotProduct = FVector::DotProduct(v1, v2);

	float angle = UKismetMathLibrary::DegAcos(holdDotProduct);

	return angle;
}


