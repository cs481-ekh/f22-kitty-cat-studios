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
	BigPoof UMETA(DisplayName = "BigPoof"),
	Health UMETA(DisplayName = "Health"),
	DamageUp UMETA(DisplayName = "DamageUp"),
	Speed UMETA(DisplayName = "Speed"),
	KillBall UMETA(DisplayName = "KillBall"),
	Sponge UMETA(DisplayName = "Sponge")
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pool, meta = (AllowPrivateAccess = "true"))
	UObjectPool* HealthPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pool, meta = (AllowPrivateAccess = "true"))
	UObjectPool* DamageUpPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pool, meta = (AllowPrivateAccess = "true"))
	UObjectPool* SpeedPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pool, meta = (AllowPrivateAccess = "true"))
	UObjectPool* KillBallPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pool, meta = (AllowPrivateAccess = "true"))
	UObjectPool* SpongePool;

public:		// Static API

	// Singleton instance
	static AParticleSpawner* SingletonInstance;

	// Interface
	static class APoolableObject* SpawnParticle(ParticleType particle, const FVector& worldLocation, const FVector& worldVelocity, float lifespan = 0);
};
