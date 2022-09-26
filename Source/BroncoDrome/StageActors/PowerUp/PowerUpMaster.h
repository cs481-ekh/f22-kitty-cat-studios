#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpMaster.generated.h"

UCLASS()
class BRONCODROME_API APowerUpMaster : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUpMaster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetTimeTillExpiration();



public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	FTimerHandle PowerUpStatusHandler;

	float timeTracker = 1.5f; //This is the value for the time the power up lasts

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float BobTimeScale = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float BobDistanceScale = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float RotationScale = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PowerUpAbility, meta = (AllowPrivateAccess = "true"))
		int powerTypeIndex; //What power it is. This is changed in the Unreal Engine Editor, not code.


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

	FDateTime spawnTime;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
