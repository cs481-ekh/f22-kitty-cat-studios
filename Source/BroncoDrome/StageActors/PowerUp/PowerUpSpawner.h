// // Copyright (C) Team Gregg 2022. All Rights Reserved.


#include "PowerUpSpawner.generated.h"

UCLASS()
class BRONCODROME_API APowerUpSpawner : public AActor
{
	GENERATED_BODY()

public:
	APowerUpSpawner();
	
	UPROPERTY(EditAnywhere, Category = "Spawing PowerUP")
		TSubclassOf<class APowerUpMaster> powerUpClass;

	//Mesh
	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* PowerUpSpawnerMeshComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void spawnPowerUp(FVector loc, FRotator rot);
	

private:
	
};

