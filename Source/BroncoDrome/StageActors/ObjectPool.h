// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableObject.h"
#include "ObjectPool.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BRONCODROME_API UObjectPool : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UObjectPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:		// Blueprint components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ObjectPool, meta = (DisplayName = "Prefab", AllowPrivateAccess = "true"))
	TSubclassOf<APoolableObject> Prefab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ObjectPool, meta = (DisplayName = "Pool Size", AllowPrivateAccess = "true"))
	int PoolSize = 100;

private:	// Members

	TArray<APoolableObject*> m_Pool;
	int32 m_PoolIndex = 0;

public:		// Interface

	APoolableObject* SpawnObject(const FVector& worldLocation, const FVector& worldVelocity, float lifespan = 0);

};
