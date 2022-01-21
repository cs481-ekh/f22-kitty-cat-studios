// Copyright (C) Dromies 2021. All Rights Reserved.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Print name to screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Hello from %s (the rotating tire)!"), *GetDebugName(this)));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Animate the object
	Animate(DeltaTime);

}

void ATestActor::Animate(float DeltaTime)
{
	// Bob object up and down over time
	AddActorLocalOffset(FVector(0.f, 0.f, FGenericPlatformMath::Cos(GetGameTimeSinceCreation() * BobTimeScale) * BobDistanceScale));

	// Rotate object over time
	AddActorLocalRotation(FRotator(0.f, RotationScale * DeltaTime, 0.f));

}
