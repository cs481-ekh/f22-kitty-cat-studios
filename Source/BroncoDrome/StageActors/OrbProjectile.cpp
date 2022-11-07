// Copyright (C) Dromies 2021. All Rights Reserved.
// // Copyright (C) Team Gregg 2022. All Rights Reserved.

#include "OrbProjectile.h"
#include "../Runner/Runner.h"
// Sets default values
AOrbProjectile::AOrbProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	init();

	//Despawn after 5s
	InitialLifeSpan = 5.0f;
	//Damage
	shotDamage = 20; //Can now set damage
}
void AOrbProjectile::FireOrbInDirection(const FVector& Direction, AActor* Runner)
{
	UE_LOG(LogTemp, Display, TEXT("fire orb in direction: %s"), *(Direction.ToString()));
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	RunnerParent = Runner; 
}
// Called when the game starts or when spawned
void AOrbProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void AOrbProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Destroyed after 5 seconds?
	if (ProjectileMovementComponent->Velocity.Size() < 5.0) {
		Destroy();
	}
}
void AOrbProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics()){
		if (ARunner* runner = Cast<ARunner, AActor>(OtherActor)) {
			runner->hitMe(shotDamage * (-1), RunnerParent);//(Make it negative) //hitMe function lets runner deal with the powerups
			//runner->AddToHealth(shotDamage * (- 1)); //(Make it negative) Left here incase something breaks
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit a runner."), *GetDebugName(this)));
			Cast<ARunner, AActor>(RunnerParent)->AddToScore(10);
		}
		Destroy();
    }
}

void AOrbProjectile::init() {
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent) {
		//Create sphere collider
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		//Set collision profile
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &AOrbProjectile::OnHit);
		CollisionComponent->InitSphereRadius(25.0f);
		//Make root
		RootComponent = CollisionComponent;
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if (!ProjectileMovementComponent) {
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//Both speed = 5000 means consistent speed
		ProjectileMovementComponent->InitialSpeed = 8000.0f;
		ProjectileMovementComponent->MaxSpeed = 8000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		//False = destroyed on impact with wall
		//ProjectileMovementComponent->Bounciness = 0.1f;
		//0 = No gravity
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent) {
		//Create mesh
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Assets/Blaster/LaserShot")); ///Game/Assets/Blaster/PlasmaBall

		if (Mesh.Succeeded()) {
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
		//Add material
		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/Assets/Blaster/MaterialAndTextures/Plasma")); 
		if (Material.Succeeded()) {
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		}
		ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		ProjectileMeshComponent->SetupAttachment(RootComponent);
		ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
