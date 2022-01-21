// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolableObject.generated.h"

UCLASS()
class BRONCODROME_API APoolableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:	// Members

	FTimerHandle m_LifespanTimer;
	FVector m_Velocity;

	// Dumb workaround
	void DeactivateAfterTime();

public:		// Interface

	void SetActive(bool active);
	void SetLifespan(float lifespan);
	void SetLocation(const FVector& location);
	void SetSpeed(float speed);
	void SetDirection(const FVector& direction);
	void SetVelocity(const FVector& velocity);

};
