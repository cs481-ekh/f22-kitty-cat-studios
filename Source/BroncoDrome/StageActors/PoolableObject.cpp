// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "PoolableObject.h"

// Sets default values
APoolableObject::APoolableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void APoolableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoolableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move based on velocity
	SetActorLocation(GetActorLocation() + m_Velocity *  DeltaTime);
}

void APoolableObject::DeactivateAfterTime()
{
	if (!m_LifespanTimer.IsValid())
		return;

	SetActive(false);
}

void APoolableObject::SetActive(bool active)
{
	SetActorHiddenInGame(!active);
}

void APoolableObject::SetLifespan(float lifespan)
{
	m_LifespanTimer.Invalidate();

	if (lifespan > 0.f)
		GetWorldTimerManager().SetTimer(m_LifespanTimer, this, &APoolableObject::DeactivateAfterTime, lifespan, false);
}

void APoolableObject::SetLocation(const FVector& location)
{
	// I know this looks stupid, but it's the only way to make it work.
	// Might have something to do with passing by reference?
	SetActorLocation(location);
	SetActorRelativeLocation(FVector(location.X, location.Y, location.Z));
}

void APoolableObject::SetSpeed(float speed)
{
	m_Velocity = m_Velocity.GetSafeNormal() * speed;
}

void APoolableObject::SetDirection(const FVector& direction)
{
	const float speed = m_Velocity.Size();
	m_Velocity = direction.GetSafeNormal() * speed;
}

void APoolableObject::SetVelocity(const FVector& velocity)
{
	m_Velocity = velocity;
}

