// // Copyright (C) Dromies 2021. All Rights Reserved.

#include "OrbProjectile.h"

#include "../Runner/Runner.h"

// Sets default values
AOrbProjectile::AOrbProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	}

	if (!ProjectileMovementComponent) {
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 5000.0f;
		ProjectileMovementComponent->MaxSpeed = 10000000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.1f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.25f;
	}

	if(!ProjectileMeshComponent){
		//Create mesh
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Assets/Blaster/Sphere.Sphere"));

		if(Mesh.Succeeded()){
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}

		//Add material
		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Engine/MapTemplates/Materials/BasicAsset02.BasicAsset02"));
		if(Material.Succeeded()){
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		}

		ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		ProjectileMeshComponent->SetupAttachment(RootComponent);
	}

	//Despawn after 5s
	InitialLifeSpan = 5.0f;
}

void AOrbProjectile::FireOrbInDirection(const FVector& Direction, AActor* Runner)
{
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

	if (ProjectileMovementComponent->Velocity.Size() < 5.0) {
		Destroy();
	}
}

void AOrbProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics()){
		if (ARunner* runner = Cast<ARunner, AActor>(OtherActor)) {
			runner->AddToHealth(-20);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit a runner."), *GetDebugName(this)));
			Cast<ARunner, AActor>(RunnerParent)->AddToScore(10);
		}

		Destroy();
    }

}

