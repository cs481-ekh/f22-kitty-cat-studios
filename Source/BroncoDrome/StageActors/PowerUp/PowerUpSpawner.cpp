// // Copyright (C) Team Gregg 2022. All Rights Reserved.


#include "PowerUpSpawner.h"

APowerUpSpawner::APowerUpSpawner() {
	//Create mesh
	PowerUpSpawnerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Blueprints/StageActors/PowerUps/PowerUpModels/PowerUpSpawnerBaseExport"));
	if (Mesh.Succeeded()) {
		PowerUpSpawnerMeshComponent->SetStaticMesh(Mesh.Object);
	}
	//Add material
	/*
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/Blueprints/StageActors/PowerUps/PowerUpModels/PowerUpMaterials/"));
	if (Material.Succeeded()) {
		PowerUpSpawnerMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, PowerUpSpawnerMaterialInstance);
	}
	PowerUpSpawnerMeshComponent->SetMaterial(0, PowerUpSpawnerMaterialInstance);
	PowerUpSpawnerMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	PowerUpSpawnerMeshComponent->SetupAttachment(RootComponent);
	*/


}

// Called when the game starts or when spawned
void APowerUpSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APowerUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUpSpawner::spawnPowerUp(FVector loc, FRotator rot)
{
	FActorSpawnParameters SpawnParams;
	//auto World = GetWorld();
	//APowerUpMaster* powerUp = GetWorld()->SpawnActor<APowerUpMaster>(powerUpClass, loc, rot, SpawnParams);


}
