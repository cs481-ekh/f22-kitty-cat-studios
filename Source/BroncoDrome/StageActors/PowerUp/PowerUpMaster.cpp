
#include "PowerUpMaster.h"
#include "../../StageActors/Spawner.h"
#include "../../Runner/Runner.h"
#include "../../StageActors/ParticleSpawner.h"

// Sets default values
APowerUpMaster::APowerUpMaster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void APowerUpMaster::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUpMaster::ShowExpiration, 9.0f, false);

	Mesh->SetGenerateOverlapEvents(true);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APowerUpMaster::ExecuteFunction);

	spawnTime = FDateTime::Now();
}

// Called every frame
void APowerUpMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Animate(DeltaTime);
}

void APowerUpMaster::Animate(float DeltaTime)
{
	// Bob object up and down over time
	if (timeTracker > 0)
		AddActorLocalOffset(FVector(0.f, 0.f, FGenericPlatformMath::Cos(GetGameTimeSinceCreation() * BobTimeScale) * BobDistanceScale));
	else
		AddActorLocalOffset(FVector(0.f, 0.f, 9.0f));

	// Rotate object over time
	AddActorLocalRotation(FRotator(0.f, RotationScale * DeltaTime, 0.f));
}

void APowerUpMaster::ShowExpiration()
{
	GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUpMaster::HideActor, timeTracker, true);
}

void APowerUpMaster::HideActor()
{
	//Ensure the timer doesn't change length between disappear reappear cycles.
	if (!hidden)
	{
		timeTracker -= 0.1f;
		GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUpMaster::HideActor, timeTracker, true);
	}

	hidden = !hidden;

	SetActorHiddenInGame(hidden);

	if (timeTracker <= 0)
		SetActorEnableCollision(false);

	SetActorTickEnabled(!hidden);
}

void APowerUpMaster::ExecuteFunction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
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
		GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUpMaster::HideActor, 1.5f, true);
		RotationScale = 600.0f;

		//Add various calls to car methods in this switch statement to accomplish power up stuff.
		switch (powerTypeIndex) {
			case 0:
				dynamic_cast<ARunner*>(OtherActor)->AddToHealth(20); //Health
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power Health"), *GetDebugName(this)));
				break;
			case 1:
				dynamic_cast<ARunner*>(OtherActor)->ThrottleInput(5.0f); //Speed
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power Speed"), *GetDebugName(this)));
				break;
			case 2:
				dynamic_cast<ARunner*>(OtherActor)->AddToDamage(10); //Damage
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power Extra Damage"), *GetDebugName(this)));
				break;
			case 3:
				dynamic_cast<ARunner*>(OtherActor)->obstainShotAbsorbPower(5); //ShotAbsorb default 5
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power ShotAbsorb"), *GetDebugName(this)));
				break;
		}

		AParticleSpawner::SpawnParticle(BigPoof, GetActorLocation(), FVector(), 1.f); //Poof it is gone
	}
	else
	{
		//Deals with relocating powerups
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Power spawned in wall, relocating..."), *GetDebugName(this)));
		UpdateLocation(FVector(FMath::RandRange(ASpawner::GetBounds().Min.X, ASpawner::GetBounds().Max.X), FMath::RandRange(ASpawner::GetBounds().Min.Y, ASpawner::GetBounds().Max.Y), -70));
	}
}

int APowerUpMaster::GetTimeTillExpiration()
{
	return 20 - (FDateTime::Now() - spawnTime).GetTotalSeconds(); //Don't touch this! I don't know what it does. The 20 value is not for the time
}

void APowerUpMaster::UpdateLocation(FVector point)
{
	//Basically resets everything on tp.
	SetActorLocation(point, false, 0, ETeleportType::None);
	timeTracker = 1.0f;
	GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUpMaster::ShowExpiration, 9.0f, false);
	spawnTime = FDateTime::Now();
	RotationScale = 90.0f;

	hidden = false;
	SetActorHiddenInGame(hidden);
	SetActorTickEnabled(!hidden);
	SetActorEnableCollision(!hidden);
}
