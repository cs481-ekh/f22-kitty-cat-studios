// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "ParticleSpawner.h"

// Alloc memory for instance
AParticleSpawner* AParticleSpawner::SingletonInstance = nullptr;

// Sets default values
AParticleSpawner::AParticleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PoofPool = CreateDefaultSubobject<UObjectPool>(TEXT("Poof Pool"));
	BigPoofPool = CreateDefaultSubobject<UObjectPool>(TEXT("Big Poof Pool"));
}

// Called when the game starts or when spawned
void AParticleSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Declare singleton
	SingletonInstance = this;
	
}

// Called every frame
void AParticleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APoolableObject* AParticleSpawner::SpawnParticle(ParticleType particle, const FVector& worldLocation, const FVector& worldVelocity, float lifespan)
{
	if (!SingletonInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Particle singleton actor instance is not in scene! Cannot spawn."));
		return nullptr;
	}

	UObjectPool* pool = nullptr;

	switch (particle)
	{
	case ParticleType::Poof:

		UE_LOG(LogTemp, Display, TEXT("POOF particle spawned at location: %s with velocity: %s and lifespan: %f"), *(worldLocation.ToString()), *(worldVelocity.ToString()), lifespan);
		pool = SingletonInstance->PoofPool;
		break;
	case ParticleType::BigPoof:
		UE_LOG(LogTemp, Display, TEXT("BIG POOF particle spawned at location: %s with velocity: %s and lifespan: %f"), *(worldLocation.ToString()), *(worldVelocity.ToString()), lifespan);
		pool = SingletonInstance->BigPoofPool;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Particle type has not been set up yet! Cannot spawn."));
		return nullptr;
	}

	auto* object = pool->SpawnObject(worldLocation, worldVelocity, lifespan);
	return object;
}
