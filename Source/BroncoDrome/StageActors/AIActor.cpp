// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "AIActor.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"
#include "AISpawner.h"



// Sets default values
AAIActor::AAIActor(): ARunner()
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
}

// Called every frame
void AAIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsGrounded()) {
		// Mover->SetThrottleInput(1.0f);
		auto location = GetActorLocation(); //Obtains the (x,y,z) vector loction of the AI

		curr_speed = FVector::Distance(location, last_location) / DeltaTime;
		if (curr_speed < max_speed && !reverse) {
			ThrottleInput(1.0f);
		}
		else {
			ThrottleInput(-1.0f);
		}
		drawTargetLine(location);
		MoveDecision(location);
		ShotDecision(location);
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
	// MoveTowardsPlayer();

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
	// auto closest_runner = ARunnerObserver::GetPlayer(*this);
	auto player_location = closest_runner->GetActorLocation();
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
			Mover->SetSteeringInput(-1.0f);
			ThrottleInput(-1.0f);
		}
		else if (dir.Equals(FVector::LeftVector)) {
			Mover->SetSteeringInput(1.0f);
			ThrottleInput(1.0f);
		}
		else if (dir.Equals(FVector::RightVector)) {
			Mover->SetSteeringInput(-1.0f);
			ThrottleInput(1.0f);
		}
		else if (dir.Equals((FVector::LeftVector + FVector::ForwardVector))) {
			Mover->SetSteeringInput(1.0f);
			ThrottleInput(-1.0f);
		}
		else if (dir.Equals((FVector::RightVector + FVector::ForwardVector))) {
			Mover->SetSteeringInput(-1.0f);
			ThrottleInput(-1.0f);
		}
	}
	else if (defensive) {
		MoveAwayFromPlayer(player_location, player_direction);
		ThrottleInput(-1.0f);
	}
	else {
		MoveTowardsPlayer(player_location, player_direction);
		ThrottleInput(1.0f);
	}

}

//=======FOR AI SHOOTING

void AAIActor::QueryLockOnEngage(){
	// Check to see if timer is valid
	if (!m_LockOnQueryTimer.IsValid() || !player_runner)
		return;
	// Query to see if other runner is visible
	if (ARunnerObserver::IsRunnerVisible(*this, *player_runner, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST))
		// Loop another engagement query timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	else
		// Start disengage timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnDisengage, LOCK_ON_NON_VISIBLE_TIME, false);
}

void AAIActor::QueryLockOnDisengage(){
	// Check to see if timer is valid
	if (!m_LockOnQueryTimer.IsValid() || !player_runner)
		return;
	// Query to see if other runner is visible
	if (ARunnerObserver::IsRunnerVisible(*this, *player_runner, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST))
		// Return to engagemenet query timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	else
		// Stop targeting, target has been invisible for too long
		player_runner = nullptr;
}
void AAIActor::LockOn(){
	// Currently locked on?
	if (!player_runner){
		// Target closest visible Runner (may return null)
		player_runner = ARunnerObserver::GetClosestRunner(*this, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST);
		// If found target, start query tick
		if (player_runner)
			GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &AAIActor::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	}
	else{
		// Clear currently locked-on Runner
		player_runner = nullptr;
		// Stop any timers that may be running
		m_LockOnQueryTimer.Invalidate();
	}
}
void AAIActor::drawTargetLine(FVector location) {
	//ARunner *player_runner = ARunnerObserver::GetPlayer(*this, 32000.f, 100.f, true);
	player_runner = ARunnerObserver::GetPlayer(*this, 32000.f, 100.f, true);
	if (player_runner) {
		ARunner::AimBlaster(player_runner, GetWorld()->GetDeltaSeconds());
		LockOn();
		// Fire();
	}
}

void AAIActor::ShotDecision(FVector location) {
	if (shotCount < shot_rate) { //shot timer (currently set to one shot every 30 frames
		shotCount++;
		return;
	}
	shotCount = 0; //Reset timer
	// drawTargetLine(location);
	Fire();
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
			Projectile->FireOrbInDirection(rot.Vector(), this);
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

	auto turn_rotation = UKismetMathLibrary::FindLookAtRotation(player_location, curr_location);
	auto turn_angle_manhattan = turn_rotation.GetManhattanDistance(curr_direction);

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


