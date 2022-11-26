
#include "PowerUpMaster.h"
#include "../../StageActors/Spawner.h"
#include "../../Runner/Runner.h"
#include "../../StageActors/ParticleSpawner.h"

#include "Sound/SoundCue.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
APowerUpMaster::APowerUpMaster()
{
	// Setup powerup sound effects
	static ConstructorHelpers::FObjectFinder<USoundCue> speedCue(
		TEXT("'/Game/Assets/Sound/Powerups/speed_Cue.speed_Cue'"));
    speedAudioCue = speedCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> healthCue(
        TEXT("'/Game/Assets/Sound/Powerups/health_Cue.health_Cue'"));
    healthAudioCue = healthCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> damageUpCue(
        TEXT("'/Game/Assets/Sound/Powerups/damageUp_Cue.damageUp_Cue'"));
    damageUpAudioCue = damageUpCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> killBallCue(
        TEXT("'/Game/Assets/Sound/Powerups/killBall_Cue.killBall_Cue'"));
    killBallAudioCue = killBallCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> spongeCue(
        TEXT("'/Game/Assets/Sound/Powerups/sponge_Cue.sponge_Cue'"));
    spongeAudioCue = spongeCue.Object; 

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void APowerUpMaster::BeginPlay()
{
	Super::BeginPlay();

	if (canExpire)
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

	if (timeTracker <= 0) {
		SetActorEnableCollision(false);
		Destroy(); //Remove actor when its time is up.
	}

	SetActorTickEnabled(!hidden);
}

void APowerUpMaster::ExecuteFunction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->IsA(ARunner::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("You collected a powerup."), *GetDebugName(this)));
		timeTracker = 0.0f;
		hidden = false;
		SetActorHiddenInGame(false);
		SetActorEnableCollision(false);
		SetActorTickEnabled(true);
		if(canExpire)
			GetWorldTimerManager().SetTimer(PowerUpStatusHandler, this, &APowerUpMaster::HideActor, 1.5f, true);
		RotationScale = 600.0f;

		FString selectedPowerUp = FString("");

		//Add various calls to car methods in this switch statement to accomplish power up stuff.
		switch (powerTypeIndex) {
			case 0:
				dynamic_cast<ARunner*>(OtherActor)->AddToHealth(20, false); //Health
				dynamic_cast<ARunner*>(OtherActor)->PlaySound(healthAudioCue);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power Health"), *GetDebugName(this)));
				selectedPowerUp = FString("HEALTH");
				break;
			case 1:
				dynamic_cast<ARunner*>(OtherActor)->EnableSpeedBoost(10.0f); // Enable speed boost for 10 seconds
                dynamic_cast<ARunner*>(OtherActor)->PlaySound(speedAudioCue);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power Speed"), *GetDebugName(this)));
				selectedPowerUp = FString("SPEED");
				break;
			case 2:
				dynamic_cast<ARunner*>(OtherActor)->AddToDamage(10); //Damage
				dynamic_cast<ARunner*>(OtherActor)->PlaySound(damageUpAudioCue);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power Extra Damage"), *GetDebugName(this)));
				selectedPowerUp = FString("EXTRA DAMAGE");
				break;
			case 3:
				dynamic_cast<ARunner*>(OtherActor)->obstainShotAbsorbPower(5); //ShotAbsorb default 5
				dynamic_cast<ARunner*>(OtherActor)->PlaySound(spongeAudioCue);				
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power ShotAbsorb"), *GetDebugName(this)));
				selectedPowerUp = FString("SHOT ABSORB");
				break;
			case 4:
				dynamic_cast<ARunner*>(OtherActor)->obstainKillBallPower(1); //KillBall default 1
				dynamic_cast<ARunner*>(OtherActor)->PlaySound(killBallAudioCue);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collected power KillBall!"), *GetDebugName(this)));
				selectedPowerUp = FString("KILL BALL");
				break;
			default:
				AParticleSpawner::SpawnParticle(BigPoof, GetActorLocation(), FVector(), 1.f);
				break;

		}
		powerupSpawner->SpawnParticles();
		if(dynamic_cast<ARunner*>(OtherActor)->IsPlayerControlled())
		{
			ARunnerHUD *HUD = dynamic_cast<ARunner*>(OtherActor)->GetRunnerHUD();
			HUD->ShowPowerupWidget(selectedPowerUp);			
		}
        
		powerupSpawner->PowerUpDestroyed();
		Destroy(); //Remove actor when picked up
	}

}

void APowerUpMaster::SetSpawner(APowerUpSpawner* spawner) 
{
  powerupSpawner = spawner;
}

int APowerUpMaster::GetTimeTillExpiration()
{
	return 20 - (FDateTime::Now() - spawnTime).GetTotalSeconds(); //Don't touch this! I don't know what it does. The 20 value is not for the time
}

