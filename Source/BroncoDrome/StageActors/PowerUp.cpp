// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "PowerUp.h"
#include "Spawner.h"
#include "../Runner/Runner.h"

#include "ParticleSpawner.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUp::ShowExpiration, 9.0f, false);

	Mesh->SetGenerateOverlapEvents(true);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::ExecuteFunction);

	powerTypeIndex = FMath::RandRange(0, 1);
	spawnTime = FDateTime::Now();
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Animate(DeltaTime);
}

void APowerUp::Animate(float DeltaTime)
{
	// Bob object up and down over time
	if(timeTracker > 0)
		AddActorLocalOffset(FVector(0.f, 0.f, FGenericPlatformMath::Cos(GetGameTimeSinceCreation() * BobTimeScale) * BobDistanceScale));
	else
		AddActorLocalOffset(FVector(0.f, 0.f, 9.0f));

	// Rotate object over time
	AddActorLocalRotation(FRotator(0.f, RotationScale * DeltaTime, 0.f));
}

void APowerUp::ShowExpiration()
{
	GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUp::HideActor, timeTracker, true);
}

void APowerUp::HideActor()
{
		//Ensure the timer doesn't change length between disappear reappear cycles.
		if (!hidden)
		{
			timeTracker -= 0.1f;
			GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUp::HideActor, timeTracker, true);
		}	
		
		hidden = !hidden;

		SetActorHiddenInGame(hidden);

		if(timeTracker <= 0)
			SetActorEnableCollision(false);

		SetActorTickEnabled(!hidden);
}

void APowerUp::ExecuteFunction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ARunner::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("You collected a powerup."), *GetDebugName(this)));
		timeTracker = 0.0f;
		hidden = false;
		SetActorHiddenInGame(false);
		SetActorEnableCollision(false);
		SetActorTickEnabled(true);
		GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUp::HideActor, 1.5f, true);
		RotationScale = 600.0f;
		switch (powerTypeIndex)
		{
		case 0:
			dynamic_cast<ARunner*>(OtherActor)->AddToHealth(20);
			break; //Add various calls to car methods in this switch statement to accomplish power up stuff.
		case 1: 
			dynamic_cast<ARunner*>(OtherActor)->ThrottleInput(5.0f);
			break;
		}

		AParticleSpawner::SpawnParticle(BigPoof, GetActorLocation(), FVector(), 1.f);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Power spawned in wall, relocating..."), *GetDebugName(this)));
		UpdateLocation(FVector(FMath::RandRange(ASpawner::GetBounds().Min.X, ASpawner::GetBounds().Max.X), FMath::RandRange(ASpawner::GetBounds().Min.Y, ASpawner::GetBounds().Max.Y), -70));
	}
}

int APowerUp::GetTimeTillExpiration()
{
	return 20 - (FDateTime::Now() - spawnTime).GetTotalSeconds();
}

void APowerUp::UpdateLocation(FVector point)
{
	//Basically resets everything on tp.
	SetActorLocation(point, false, 0, ETeleportType::None);
	timeTracker = 1.0f;
	GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUp::ShowExpiration, 9.0f, false);
	powerTypeIndex = FMath::RandRange(0, 1);
	spawnTime = FDateTime::Now();
	RotationScale = 90.0f;
	
	hidden = false;
	SetActorHiddenInGame(hidden);
	SetActorTickEnabled(!hidden);
	SetActorEnableCollision(!hidden);
}



