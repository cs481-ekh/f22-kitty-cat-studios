// // Copyright (C) Dromies 2021. All Rights Reserved.
// // Copyright (C) Team Gregg 2022. All Rights Reserved.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "OrbProjectile.generated.h"

UCLASS()
class BRONCODROME_API AOrbProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Handler to call when we hit an object. (will) deal damage to runners it impacts.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	//Can set damage based on players damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShotDamage, meta = (AllowPrivateAccess = "true"))
	int shotDamage;
	
	//Collision
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	//Mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	//Movement
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	//Material
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	//Set velocity of orb in given direction
	void FireOrbInDirection(const FVector& Direction, AActor* Runner);

private: 
	AActor *RunnerParent; 

	virtual void init(); //Just to set things up
};
