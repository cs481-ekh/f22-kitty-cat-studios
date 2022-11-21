// // Copyright (C) Dromies 2021. All Rights Reserved.
// // Copyright (C) Team Gregg 2022. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ARunnerHUD.h"
#include "../StageActors/AISpawnerController.h"
#include "Runner.generated.h"

// ----------------------------------------------------------------------
// For use by ARunnerVehicle::RunnerState Child Implementations			-
#define StateImpl(CLASS_TYPE)                                  \
public: /* Constructors */                                     \
	CLASS_TYPE() : RunnerState() {}                            \
	CLASS_TYPE(class ARunner *runner) : RunnerState(runner) {} \
                                                               \
public: /* Overrides */                                        \
	virtual void OnEnter() override;                           \
	virtual void OnExit() override;                            \
	virtual void Tick(float DeltaTime) override;               \
	// ----------------------------------------------------------------------

UCLASS()
class BRONCODROME_API ARunner : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public: // Constructors
	ARunner();

protected: // Inherrited
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public: // Public inherrited
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

public: // Components
	// Spring arm used to control collision correction and swinging of Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *SpringArm;

	// Camera pointed at the runner and vehicle direction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *Camera;

	// Main physics-simulated mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent *RootMesh;

	// Mesh in addition to frame
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *BodyMesh;

	// Movement component for vehicle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UChaosWheeledVehicleMovementComponent *Mover;

	// Base of the blaster (just a sphere)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *BlasterBase;

	// Cannon/pointer of the blaster (just a rotatable cylinder)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *BlasterCannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface *HitMaterial;

	UPROPERTY()
		FTimerHandle TimerHandle;

protected:
	//Projectiles to use
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AOrbProjectile> ProjectileClass; //Regular orb whose damage is set by the player's damage (affected by power ups)
	UPROPERTY(EditAnywhere, Category = Projectile) 
		TSubclassOf<class AKillBallProjectile> KillBallProjectileClass; //A one shot orb to kill any unguarded runner


public: // Constants
	// Camera
	const float CAMERA_PIVOT_HEIGHT = 120.f;
	const float CAMERA_ROTATION_OFFSET = -10.f;
	const float CAMERA_ASPECT_RATIO = 16.9 / 9.f;

	// Input
	const float LOOK_BEHIND_THRESHOLD = .7f;
	const float HOP_FORCE_AMOUNT = 68000.f;
	const float ROTATION_FIX_THRESHOLD = 32.5f;
	const float ROTATION_FIX_FORCE = 1000.f;

	// Raycasts
	const float GROUNDING_RAYCAST_DIST = 25.f;
	const FCollisionQueryParams GROUNDING_COLLISION_PARAMS = FCollisionQueryParams(FName("RunnerGroundCaster"), false, this);

	// Lock-on
	const float LOCK_ON_QUERY_TIME = 1.f;
	const float LOCK_ON_NON_VISIBLE_TIME = 3.f;
	const float LOCK_ON_DISTANCE = 32000.f;
	const float LOCK_ON_FIELD_OF_VIEW = 100.f;
	const bool LOCK_ON_RAYCAST_TEST = true;
	const float LOCK_ON_BLASTER_RPS = 12.56637; // Rotation of blaster, radians per second (4*Pi for now)

public: // Attributes
	// Torque input
	FVector m_TorqueInput = FVector();

	// Camera
	bool m_IsLookingBehind = false;
	class ARunner *m_CameraTargetRunner = nullptr;
	FTimerHandle m_LockOnQueryTimer;

	int score = 0;
	int health = 100; // out of 100
	int lives = 3;	// out of 3
	int playerDamage = 20; //Default damage
	int gameTime = 180; //Time per level, in seconds
	int AIToKill = 3;
	FTimerHandle GameTimeHandler; //For tick
	FTimerHandle ShotTimerHandler;
	FTimerHandle AimTimerHandler;
	FTimerHandle RespawnTimerHandler;
	bool canFire = true;
	float shotTimerCooldown = 0.3f;
	bool canAim = true;
	float aimTimerCooldown = 0.05f;
	FVector lastAimHitPoint;
	bool autoTarget = false; // if the runner will automatically target instead of manual targeting
	bool initialized = false; // if player is initialized

	//KillBall
	bool killBallOn;
	int killBallShots;
	//ShotAbsorb
	bool shotAbsorbOn;
	int shotAbsorbHits;
	//Designate Player-AI
	bool isAI = false;
	FTimerHandle RunnerStatusHandler;

private:

	TArray<UMaterialInterface *> originalMaterials;
	bool isRed = false;

public: // Sound
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue *thudAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue *jumpAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue* laserAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue* spongeBreakAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue* spongeTinkAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue* runnerHitAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue* runnerExplosionAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* engineAudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundAttenuation *attenuation;

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio")
	void ChangeMIntensity(int intensity);

private: // HUD
	ARunnerHUD *HUD;
	float speedUpdateTimer = 0; 

public:
	ARunnerHUD* GetRunnerHUD();

public: // Input functions
	void ThrottleInput(float in);
	void SteeringInput(float in);

private:
	void ReinstateAll();
	void CameraInput(float in);
	void HandleLookBehindInput(float in);
	void LookBehind();
	void ResetCamera();
	void RotateInput(float in);
	void Hop();
	void LockOn();
	void Fire();
	void QueryLockOnEngage();
	void QueryLockOnDisengage();
	void Pause();
	void FlashRed();
	UFUNCTION(BlueprintCallable)
	void SkipCutscene();
	AAISpawnerController* spawnController;

public:
	bool IsGrounded();
	void FixRotation();
	void DecrementGameTime();
	void DecrementAILeftToKill();
	void CheckForGameOver();
	void AddToScore(int newScore);  //Changes score
	void SetCanFire();
	void SetCanAim();

	//Power ups
	void hitMe(int damage, AActor* shotOrigin); //Holds the needed steps to deal damage based on current powerups
	void AddToHealth(int newHealth, bool damageOriginatedFromPlayer); //Changes health
	void Respawn();	// For respawning
	void AddToDamage(int addedDamage); //Can change damage
	void obstainShotAbsorbPower(int hits); //ShotAbsorb 
	void obstainKillBallPower(int shots); //KillBall
	void EnableSpeedBoost(float duration); // speed boost
	void DisableSpeedBoost();
	bool speedBoost = false;
	FTimerHandle SpeedBoostTimerHandler;
	float defaultSpeed = 0.2f; // This is the default throttle input (60%) when not under effect of speed boost powerup (max speed is 1.0 or 100% of throttle input) this must be less than 1.0 so that the speed power up will work

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ARunner)
	void SpawnParticles();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ARunner)
	void Visible(bool visible);

	void AimBlaster(const FVector targetLocation, const float deltaTime);

//Displays for winning and losing
private:
	void WinScreen();
	void LoseScreen();

public: // Sound Functions
	void PlaySound(USoundCue* cue);

public: // Sound Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MusicIntensity = 1;

private: // State machines
	void InitStateMachines();

	class RunnerState
	{
	private: // Members
		class ARunner *m_Runner;

	public: // Constructor
		inline RunnerState() : m_Runner(nullptr) {}
		inline RunnerState(class ARunner *runner) : m_Runner(runner) {}
		virtual inline ~RunnerState() {}

	public: // Pure-virtual
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;
		virtual void Tick(float DeltaTime) = 0;

	protected: // Inherrited Interface
		inline ARunner *GetRunner()
		{
			verifyf(m_Runner != nullptr, TEXT("Owner of this RunnerState is null!"));
			return m_Runner;
		}
	};

	class RunnerStateMachine
	{
	private: // Members
		RunnerState *m_CurrentState = nullptr;

	public: // Constructors/Initializer
		inline void Init(RunnerState *initialState)
		{
			m_CurrentState = initialState;
			m_CurrentState->OnEnter();
		};

	public: // Interface
		inline void TickCurrentState(float DeltaTime)
		{
			m_CurrentState->Tick(DeltaTime);
		};

		inline void TransitionState(RunnerState *toState)
		{
			if (m_CurrentState == nullptr || toState == nullptr || m_CurrentState == toState)
				return;

			m_CurrentState->OnExit();
			m_CurrentState = toState;
			m_CurrentState->OnEnter();
		};

		inline const bool IsInState(const RunnerState *state)
		{
			return m_CurrentState == state;
		};
	};

	// Arial SM ------------------------------------------------------

	RunnerStateMachine AerialSM;

	class AerialGroundedState : public RunnerState{
									StateImpl(AerialGroundedState)

								} AerialS_Grounded;

	class AerialAirState : public RunnerState{
							   StateImpl(AerialAirState)

						   } AerialS_Air;

	// Movement SM ------------------------------------------------------

	RunnerStateMachine MovementSM;

	class MovementStraightState : public RunnerState{
									  StateImpl(MovementStraightState)

								  } MovementS_Straight;

	class MovementDriftState : public RunnerState{
								   StateImpl(MovementDriftState)

							   } MovementS_Drift;
};
