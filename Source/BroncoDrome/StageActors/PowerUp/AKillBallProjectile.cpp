// Copyright (C) Dromies 2021. All Rights Reserved.
#include "KillBallProjectile.h"

// Sets default values
AKillBallProjectile::AKillBallProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	init();

	//Despawn after 5s
	InitialLifeSpan = 5.0f;
	//Damage
	shotDamage = 9001; //HardSet damage to meme
}

// Called when the game starts or when spawned
void AKillBallProjectile::BeginPlay()
{
	Super::BeginPlay();

}
// Called every frame
void AKillBallProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Destroyed after 5 seconds?
	if (ProjectileMovementComponent->Velocity.Size() < 5.0) {
		Destroy();
	}
}
//OnHit function should be given from parent

void AKillBallProjectile::init() {
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent) {
		//Create sphere collider
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		//Set collision profile
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &AOrbProjectile::OnHit);
		CollisionComponent->InitSphereRadius(40.0f); //Nearly double for extra chance of hit
		//Make root
		RootComponent = CollisionComponent;
	}
	if (!ProjectileMovementComponent) {
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//Both speed = 9001 means a faster consistent speed
		ProjectileMovementComponent->InitialSpeed = 9001.0f;
		ProjectileMovementComponent->MaxSpeed = 9001.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		//False = destroyed on impact with wall
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.1f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.25f;
		//0 = No gravity
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	//If it does not exist make it
	if (!ProjectileMeshComponent) { //This is needed if the killball was the first ever shot of the player. Else it crashes
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	}

	//Overrides the base model and materials
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Assets/Blaster/KillBall"));
	if (Mesh.Succeeded()) {
		ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
	}
	//Add material
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/Assets/Blaster/MaterialAndTextures/KillBall"));
	if (Material.Succeeded()) {
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

}
