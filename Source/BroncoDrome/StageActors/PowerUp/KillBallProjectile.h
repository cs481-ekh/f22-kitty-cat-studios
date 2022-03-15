// // Copyright (C) Team Gregg 2022. All Rights Reserved.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../OrbProjectile.h"
#include "KillBallProjectile.generated.h"

UCLASS()
class BRONCODROME_API AKillBallProjectile : public AOrbProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKillBallProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	virtual void init() override;
};

