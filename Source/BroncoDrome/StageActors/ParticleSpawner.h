// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool.h"
#include "ParticleSpawner.generated.h"

UENUM()
enum ParticleType
{
	Poof UMETA(DisplayName = "Poof"),
	BigPoof UMETA(DisplayName = "BigPoof")
};

UCLASS()
class BRONCODROME_API AParticleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:		// Blueprint Members

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pool, meta = (AllowPrivateAccess = "true"))
	UObjectPool* PoofPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pool, meta = (AllowPrivateAccess = "true"))
	UObjectPool* BigPoofPool; 

public:		// Static API

	// Singleton instance
	static AParticleSpawner* SingletonInstance;

	// Interface
	static class APoolableObject* SpawnParticle(ParticleType particle, const FVector& worldLocation, const FVector& worldVelocity, float lifespan = 0);
};
