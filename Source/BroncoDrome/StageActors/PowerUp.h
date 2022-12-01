// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"


UCLASS()
class BRONCODROME_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateLocation(FVector point);

	int GetTimeTillExpiration();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	FTimerHandle PowerUpStatusHandler;

	float timeTracker = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float BobTimeScale = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float BobDistanceScale = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float RotationScale = 90.f;

private:
	void HideActor();

	void Animate(float DeltaTime);

	void ShowExpiration();

	UFUNCTION()
	void ExecuteFunction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	bool hidden = false;

	bool gotten = false;

	int powerTypeIndex; //What power it is. currently randomly made

	FDateTime spawnTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
