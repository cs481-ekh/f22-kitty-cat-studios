// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "RunnerObserver.h"

#include "../Runner/Runner.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"


ARunnerObserver* ARunnerObserver::SingletonInstance = nullptr;

// Sets default values
ARunnerObserver::ARunnerObserver()
	: m_RunnerRegister()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARunnerObserver::BeginPlay()
{
	Super::BeginPlay();

	// Declare singleton
	SingletonInstance = this;

	// Declare members
	m_RunnerRegister.clear();
	
}

// Called every frame
void ARunnerObserver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARunnerObserver::RegisterRunner(ARunner& runner)
{
	std::set<ARunner*>& reg = SingletonInstance->m_RunnerRegister;

	if (reg.find(&runner) == reg.end())
	{
		reg.insert(&runner);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WARNING: RunnerObserver could not register "
		//	"Runner %p because it is already being managed"), &runner));
	}
}

void ARunnerObserver::DeregisterRunner(ARunner& runner)
{
	std::set<ARunner*>& reg = SingletonInstance->m_RunnerRegister;

	if (reg.find(&runner) != reg.end())
	{
		reg.erase(&runner);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WARNING: RunnerObserver could not deregister "
		//	"Runner %p because it is not being managed."), &runner));
	}
}

ARunner* ARunnerObserver::GetClosestRunner(const ARunner& fromRunner, float maxDistance, 
	float angularThreshold, bool raycastTest)
{
	const std::set<ARunner*>& reg = SingletonInstance->m_RunnerRegister;

	// Cache for closet runner + statistics
	ARunner* closestRunner = nullptr;
	float bestDistance = maxDistance;

	// Iterate through observer register
	for (ARunner* toRunner : reg)
	{
		// Is this the fromRunner?
		if (toRunner == &fromRunner)
			continue;

		// Mark as best if distance is best and visibility check passes
		const float distance = GetRunnerDistance(fromRunner, *toRunner);

		if (distance <= bestDistance &&
			IsRunnerVisible(fromRunner, *toRunner, maxDistance, angularThreshold, raycastTest))
		{
			closestRunner = toRunner;
			bestDistance = distance;
		}

	}

	// Note: Returns nullptr if no Runners fits the args
	return closestRunner;
}

ARunner* ARunnerObserver::GetPlayer(const ARunner& fromRunner, float maxDistance,
	float angularThreshold, bool raycastTest) {
	const std::set<ARunner*>& reg = SingletonInstance->m_RunnerRegister;
	for (ARunner* toRunner : reg) {
		if (toRunner == &fromRunner) 
			continue;
		if (!toRunner->isAI && IsRunnerVisible(fromRunner, *toRunner, maxDistance, angularThreshold, raycastTest)) 
			return toRunner;
	}
	return nullptr;
}

float ARunnerObserver::GetRunnerDistance(const ARunner& fromRunner, const ARunner& toRunner)
{
	if (&fromRunner == &toRunner)
		return 0.f;

	const FVector& fromRunnerLocation = fromRunner.GetActorLocation();
	const FVector& toRunnerLocation = toRunner.GetActorLocation();

	return FVector::Dist(fromRunnerLocation, toRunnerLocation);
}

float ARunnerObserver::GetAngleBetweenRunners(const ARunner& fromRunner, const ARunner& toRunner, bool fromCamera)
{
	// Forward vector of fromRunner (projected onto 2D plane)
	const FVector fromCameraForward = UKismetMathLibrary::ProjectVectorOnToPlane(
		(fromCamera ? fromRunner.Camera->GetForwardVector() : fromRunner.GetActorForwardVector()), FVector::UpVector)
		.GetSafeNormal();

	// Direction that toRunner is relative to fromRunner
	const FVector fromToForward = UKismetMathLibrary::ProjectVectorOnToPlane(
		UKismetMathLibrary::GetDirectionUnitVector(
			fromRunner.GetActorLocation(), toRunner.GetActorLocation()), FVector::UpVector)
		.GetSafeNormal();

	// The min angle offset between these two directional vectors [0.f, 180.f]
	return UKismetMathLibrary::DegAcos(FVector::DotProduct(fromCameraForward, fromToForward));
}

bool ARunnerObserver::RunnerRaycastTest(const ARunner& fromRunner, const ARunner& toRunner)
{
	// Buffer for raycast results
	FHitResult outHit;
	const FCollisionQueryParams collisionParams(FName("RunnerVisualRaycastTest"), false, &fromRunner);

	// Get fromRunner camera and toRunner locations
	const FVector& fromRunnerLocation = fromRunner.Camera->GetComponentLocation();
	const FVector& toRunnerLocation = toRunner.GetActorLocation();

	// Return true if raycast was succesful (i.e., nothing was in the way)
	return (SingletonInstance->GetWorld()->LineTraceSingleByChannel(
		outHit, fromRunnerLocation, toRunnerLocation, ECC_WorldDynamic, collisionParams) &&
		outHit.Actor == &toRunner);
}

bool ARunnerObserver::IsRunnerVisible(const ARunner& fromRunner, const ARunner& toRunner,
	float maxDistance, float angularThreshold, bool raycastTest)
{
	// Return true from and to are the same runner
	if (&fromRunner == &toRunner)
		return true;

	// Is it within viewing distance?
	const float distance = GetRunnerDistance(fromRunner, toRunner);
	if (distance > maxDistance)
		return false;

	// Is the angle valid?
	const float angle = GetAngleBetweenRunners(fromRunner, toRunner);
	if (angle > angularThreshold)
		return false;

	// Is anything obstructing the view?
	const bool raycastSuccessful = RunnerRaycastTest(fromRunner, toRunner);
	if (raycastTest && !raycastSuccessful)
		return false;

	// If all tests passed, return true
	return true;
	
}
/**
 * UWorld - current world in play
 * TArray - array to be populated with actors
 */
template <typename T> void ARunnerObserver::GetAllActorsOfClass(UWorld *World, TArray<T *> &Out) {
  for (TActorIterator<T> It(World); It; ++It)
  {
    Out.Add(*It);
  }    

}



