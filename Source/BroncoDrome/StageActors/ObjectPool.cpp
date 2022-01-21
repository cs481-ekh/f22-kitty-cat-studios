// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "ObjectPool.h"

// Sets default values
UObjectPool::UObjectPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void UObjectPool::BeginPlay()
{
	Super::BeginPlay();

	if (Prefab)
	{
		for (int i = 0; i < PoolSize; i++)
		{
			auto* object = GetWorld()->SpawnActor<APoolableObject>(Prefab, FVector(), FRotator());
			object->SetActive(false);
			m_Pool.Add(object);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPool does not have a Prefab set up in Blueprints!"));
	}
	
}

APoolableObject* UObjectPool::SpawnObject(const FVector& worldLocation, const FVector& worldVelocity, float lifespan)
{
	if (m_Pool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPool not initialized! Spawn aborted."));
		return nullptr;
	}

	// Spawn an object and set data
	auto* object = m_Pool[m_PoolIndex];
	object->SetActive(true);
	object->SetLocation(worldLocation);
	object->SetVelocity(worldVelocity);
	object->SetLifespan(lifespan);

	if (++m_PoolIndex >= m_Pool.Num())
		m_PoolIndex = 0;

	return object;
}

