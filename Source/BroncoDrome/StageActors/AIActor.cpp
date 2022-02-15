// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "AIActor.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
AAIActor::AAIActor(): ARunner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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
		auto location = GetActorLocation();

		curr_speed = FVector::Distance(location, last_location) / DeltaTime;
		if (curr_speed < max_speed && !reverse) {
			ThrottleInput(1.0f);
		}
		else {
			ThrottleInput(-1.0f);
		}

		MoveDecision();
		ShotDecision();
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
	const FCollisionQueryParams collisionParams(FName(TEXT("TestCast")), false, this);
	// collisionParams.AddIgnoredActor(*this);

	FVector start = GetActorLocation();
	bool hit = GetWorld()->LineTraceSingleByChannel(outHit, start, start + to, ECC_WorldDynamic, collisionParams);
	if (hit) {
		return &outHit;
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

	FHitResult* backward = Raycast(-GetActorForwardVector() * max_distance);
	if (backward) {
		if (backward->Distance + fwd_dist < closest) {
			closest = backward->Distance + fwd_dist;
			result = Fvector::BackwardVector;
		}
	}

	//FHitResult* bright = Raycast((GetActorRightVector() + (-GetActorForwardVector())) * max_distance);
	//if (bright) {
	//	if (bright->Distance < closest) {
	//		closest = bright->Distance;
	//		result = (FVector::RightVector + FVector::BackwardVector);
	//	}
	//}

	//FHitResult* bleft = Raycast(((-GetActorRightVector()) + (-GetActorForwardVector)) * max_distance);
	//if (bleft) {
	//	if (bleft->Distance < closest) {
	//		closest = bleft->Distance;
	//		result = (FVector::LeftVector + FVector::BackwardVector);
	//	}
	//}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::SanitizeFloat(closest));
	return result;
}

void AAIActor::MoveDecision() {

	if (count != update_rate) {
		count++;
		return;
	}
	count = 0;

	// auto player_location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	auto closest_runner = ARunnerObserver::GetClosestRunner(*this);
	auto player_location = closest_runner->GetActorLocation();
	auto player_direction = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();

	auto curr_location = GetActorLocation();
	auto curr_direction = GetActorRotation();

	auto forward_vec = GetActorForwardVector();
		// ActorGetDistanceToCollision()
	// ActorLineTraceSingle()
	auto dir = GetDirection();
	reverse = dir.Equals(FVector::ForwardVector);

	if (!dir.Equals(FVector(0.0f, 0.0f, 0.0f))) {

		if (dir.Equals(FVector::ForwardVector)) {
			Mover->SetSteeringInput(-1.0f);
			ThrottleInput(-1.0f);
		}
		else if (dir.Equals(FVector::LeftVector)) {
			Mover->SetSteeringInput(1.0f);
		}
		else if (dir.Equals(FVector::RightVector)) {
			Mover->SetSteeringInput(-1.0f);
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
	else if (FVector::Distance(player_location, curr_location) > dont_care_distance) {
		Mover->SetSteeringInput(0.0f);
	}
	else if (FVector::Distance(player_location, curr_location) > car_distance) {

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Towards"), *GetDebugName(this)));
		// MoveTowardsPlayer(player_location, player_direction);
		Mover->SetSteeringInput(0.0f);
	}
	else {
		//Marie commented out so they won't move away
		//MoveAwayFromPlayer(player_location, player_direction);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Away"), *GetDebugName(this)));
	}
}

void AAIActor::ShotDecision() {
	if (shotCount != shot_rate) { //shot timer (currently set to one shot every 30 frames
		shotCount++;
		return;
	}
	shotCount = 0; //Reset timer
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI shot action called..."), *GetDebugName(this)));
	Fire();
}
void AAIActor::Fire() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI FIRE SHOT"), *GetDebugName(this)));
	if (AIProjectileClass) {
		auto World = GetWorld();
		if (!World) return;
		//else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("getWorld error"), *GetDebugName(this)));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		const auto rot = BlasterBase->GetComponentRotation();
		const auto loc = BlasterBase->GetComponentLocation() + 150.0 * (rot.Vector());	//150 is to account for the length of the barrel

		AOrbProjectile* Projectile = World->SpawnActor<AOrbProjectile>(AIProjectileClass, loc, rot, SpawnParams);
		if (Projectile) {
			//PlaySound(laserAudioCue);
			Projectile->FireOrbInDirection(rot.Vector(), this);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("You fired your weapon."), *GetDebugName(this)));
			//AParticleSpawner::SpawnParticle(Poof, BlasterBase->GetComponentLocation(),
				//BlasterBase->GetComponentRotation().Vector() * (projectile->ProjectileMovementComponent->InitialSpeed * 0.8f), 0.8f);

		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Projectile init error"), *GetDebugName(this)));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ProjectileClass error"), *GetDebugName(this)));
	}
}

void AAIActor::MoveAwayFromPlayer(FVector player_location, FRotator player_direction) {
	auto curr_location = GetActorLocation();
	auto curr_direction = GetActorRotation();

	// FOR RANDOM MOVES IF WE WANT
	FNavLocation ResultLocation = FNavLocation();
	auto radius = 500.0;
	NavSys->GetRandomReachablePointInRadius(player_location, radius, ResultLocation);

	auto turn_rotation = UKismetMathLibrary::FindLookAtRotation(player_location, curr_location);
	auto turn_angle_manhattan = turn_rotation.GetManhattanDistance(curr_direction);
	if ((last_angle == turn_angle_manhattan && turn_angle_manhattan > min_angle) || (turn_angle_manhattan == last_angle && turn_angle_manhattan < max_angle)) {
		curr_turn = -curr_turn;
		last_angle = turn_angle_manhattan;
		Mover->SetSteeringInput(curr_turn);
	}
	else {
		if (turn_angle_manhattan < min_angle || turn_angle_manhattan > max_angle) {
			Mover->SetSteeringInput(0.0f);
		}
		else {
			Mover->SetSteeringInput(curr_turn);
		}
	}
}

void AAIActor::MoveTowardsPlayer(FVector player_location, FRotator player_direction)
{
	auto curr_location = GetActorLocation();
	auto curr_direction = GetActorRotation();

	// FOR RANDOM MOVES IF WE WANT
	FNavLocation ResultLocation = FNavLocation();
	auto radius = 500.0;
	NavSys->GetRandomReachablePointInRadius(player_location, radius, ResultLocation);
	
	auto turn_rotation = UKismetMathLibrary::FindLookAtRotation(player_location, curr_location);
	auto turn_angle_manhattan = turn_rotation.GetManhattanDistance(curr_direction);
	if (turn_angle_manhattan >= min_angle && turn_angle_manhattan <= max_angle) {
		Mover->SetSteeringInput(0.0f);
		curr_turn = -curr_turn;
	}
	else {
		Mover->SetSteeringInput(curr_turn);
	}
	last_angle = turn_angle_manhattan;
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


