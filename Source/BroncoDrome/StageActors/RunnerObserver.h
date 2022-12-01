// // Copyright (C) Dromies 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <set>
#include "PowerUp.h"
#include"PowerUp/PowerUpMaster.h"
#include"PowerUp/PowerUpSpawner.h"
#include "RunnerObserver.generated.h"


UCLASS()
class BRONCODROME_API ARunnerObserver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunnerObserver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	// Member

	// Set of all managed Runners
	std::set<class ARunner*> m_RunnerRegister;
        TArray<AActor*> powerups;
        FTimerHandle GameTimeHandler;
        int frameCount;


public:		// Public Static API

	// Singleton instance
	static ARunnerObserver* SingletonInstance;




	// Pretty much the DMV
	static void RegisterRunner(class ARunner& runner);
	static void DeregisterRunner(class ARunner& runner);



        // get all actors of class
         template<typename T>
         void GetAllActorsOfClass(UWorld* World, TArray<T*>& Out);


	// Runner visibility tests
	static class ARunner* GetClosestRunner(const class ARunner& fromRunner, float maxDistance = std::numeric_limits<float>::max(), 
	float angularThreshold = 180.f, bool raycastTest = false);
        static AActor* GetClosestPowerup(const class ARunner& fromRunner, float maxDistance = std::numeric_limits<float>::max(), 
        float angularThreshold = 180.f, bool raycastTest = false);
	static class ARunner* GetPlayer(const class ARunner& fromRunner, float maxDistance = std::numeric_limits<float>::max(),
	float angularThreshold = 180.f, bool raycastTest = false);
	static float GetRunnerDistance(const class ARunner& fromRunner, const class ARunner& toRunner);
        static float GetPowerupDistance(const class ARunner& fromRunner, const class AActor& toPowerup);

	static float GetAngleBetweenRunners(const class ARunner& fromRunner, const class ARunner& toRunner, bool fromCamera = true);
	static bool RunnerRaycastTest(const class ARunner& fromRunner, const class ARunner& toRunner);
        static float GetAngleBetweenPowerups(const class ARunner& fromRunner, const AActor& toPowerup, bool fromCamera = true);
        static bool PowerupRaycastTest(const class ARunner& fromRunner, const class AActor& toPowerup);
	static bool IsRunnerVisible(const class ARunner& fromRunner, const class ARunner& toRunner,
		float maxDistance = std::numeric_limits<float>::max(), float angularThreshold = 180.f, bool raycastTest = false);
      void UpdatePowerups();

        static bool IsPowerupVisible(const class ARunner& fromRunner, const class AActor& toRunner,
              float maxDistance = std::numeric_limits<float>::max(), float angularThreshold = 180.f, bool raycastTest = false);
        static void externalUpdatePowerup();

};
