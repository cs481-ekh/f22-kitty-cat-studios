// // Copyright (C) Team Gregg 2022. All Rights Reserved.


#include "PowerUpSpawner.generated.h"

UCLASS()
class BRONCODROME_API APowerUpSpawner : public AActor
{
	GENERATED_BODY()

public:
	APowerUpSpawner();
	
	UPROPERTY(EditAnywhere, Category = "Spawning PowerUP")
		TSubclassOf<class APowerUpMaster> powerUpClass;

	//Mesh
	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* PowerUpSpawnerMeshComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle spawnTimeHandle;
	float spawnTime = 35.0f; //Remember to adjust if you also adjusted the spawn time of the power ups!

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	APowerUpMaster* powerUp;

	UFUNCTION()
		//void spawnPowerUp(APowerUpMaster* powerUp, FVector loc, FRotator rot);
		void spawnPowerUp();

private:
	
};

