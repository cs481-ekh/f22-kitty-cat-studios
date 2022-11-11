// // Copyright (C) Team Gregg 2022. All Rights Reserved.


#include "PowerUpSpawner.h"
#include "PowerUpMaster.h"
#include <BroncoDrome/BroncoSaveGame.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

APowerUpSpawner::APowerUpSpawner() {

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create mesh
	PowerUpSpawnerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Blueprints/StageActors/PowerUps/PowerUpModels/PowerUpSpawnerBaseExport"));
	if (Mesh.Succeeded()) {
		PowerUpSpawnerMeshComponent->SetStaticMesh(Mesh.Object);
	}
	

}

// Called when the game starts or when spawned
void APowerUpSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	spawnPowerUp();

}

// Called every frame
void APowerUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//spawnPowerUp();

}

void APowerUpSpawner::PowerUpDestroyed() 
{
	GetWorldTimerManager().SetTimer(spawnTimeHandle, this, &APowerUpSpawner::spawnPowerUp, spawnTime, false);
}

void APowerUpSpawner::spawnPowerUp(/*APowerUpMaster* powerUp, FVector loc, FRotator rot, FName tag */)
{

	const auto rot = this->GetActorRotation();
	auto loc = this->GetActorLocation();
	loc.Z = loc.Z + 50; //Moves it up
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	auto World = GetWorld();		

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Power Up Spawned"), *GetDebugName(this)));
	powerUp = World->SpawnActor<APowerUpMaster>(powerUpClass, loc, rot, SpawnParams);
    powerUp->SetSpawner(this);

	if (UBroncoSaveGame* load = Cast<UBroncoSaveGame>(UGameplayStatics::LoadGameFromSlot("curr", 0))) {

		difficultySetting = load->difficultySetting;
	}

	// Using random int generator, spawns power up if
	// less than or equal to value. 
	// Feel free to change values for balancing purposes.
	int random = FMath::RandRange(1, 10);
	if (random <= 3 && difficultySetting == (TEXT("Hard"))) {

		powerUp->Destroy();
	}

}

//The real implementation for this method is inside of an actual PowerUpSpawner child blueprint
//The blueprint activates the particle system
void APowerUpSpawner::SpawnParticles_Implementation() {} 

