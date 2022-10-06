// // Copyright (C) Dromies 2021. All Rights Reserved.


#include "Runner.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

//Orbs
#include "../StageActors/OrbProjectile.h"
#include "../StageActors/PowerUp/KillBallProjectile.h"

#include "../StageActors/RunnerObserver.h"
#include "../StageActors/ParticleSpawner.h"

ARunner::ARunner()
	: AerialSM(), AerialS_Grounded(this), AerialS_Air(this),
	  MovementSM(), MovementS_Straight(this), MovementS_Drift(this)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up audio 
	static ConstructorHelpers::FObjectFinder<USoundCue> thudCue(
		TEXT("'/Game/Assets/Sound/thud_Cue.thud_Cue'")
	);
	thudAudioCue = thudCue.Object; 

	static ConstructorHelpers::FObjectFinder<USoundCue> jumpCue(
		TEXT("'/Game/Assets/Sound/jump_Cue.jump_Cue'")
	);
	jumpAudioCue = jumpCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> laserCue(
		TEXT("'/Game/Assets/Sound/laser_Cue.laser_Cue'")
	);
	laserAudioCue = laserCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> engineCue(
		TEXT("'/Game/Assets/Sound/Engine/EngineCue.EngineCue'")
	);

	engineAudioComponent = CreateDefaultSubobject<UAudioComponent>(
		TEXT("EngineAudioComponent")
	);
	engineAudioComponent->SetSound(engineCue.Object);

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> attenuationReference(
		TEXT("'/Game/Assets/Sound/StadiumAttenuation.StadiumAttenuation'")
	);
	attenuation = attenuationReference.Object;

	// Init spring
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Runner Camera Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, CAMERA_PIVOT_HEIGHT));
	SpringArm->SetRelativeRotation(FRotator(0.f, CAMERA_ROTATION_OFFSET, 0.f));
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	// Init camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Runner Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bConstrainAspectRatio = true;
	Camera->AspectRatio = CAMERA_ASPECT_RATIO;

	// Init main skeletal mesh
	RootMesh = GetMesh();
	RootMesh->SetSimulatePhysics(true);

	// Init body mesh
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootMesh);

	// Init vehicle mover
	Mover = (UChaosWheeledVehicleMovementComponent*)GetMovementComponent();
	Mover->SetThrottleInput(1.0f);
	Mover->bReverseAsBrake = true;

	// Init blaster
	BlasterBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blaster Base"));
	BlasterBase->SetupAttachment(BodyMesh);
	BlasterBase->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	BlasterBase->SetWorldScale3D(FVector(0.5f));
	BlasterBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BlasterCannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blaster Cannon"));
	BlasterCannon->SetupAttachment(BlasterBase);
	BlasterCannon->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
	BlasterCannon->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	BlasterCannon->SetRelativeScale3D(FVector(0.5f, 0.5f, 2.f));
	BlasterCannon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Init engine sound
	engineAudioComponent->AttenuationSettings = attenuation;
	engineAudioComponent->bAutoManageAttachment = true;
	engineAudioComponent->SetupAttachment(GetRootComponent());
	engineAudioComponent->AutoAttachParent = GetRootComponent();
	engineAudioComponent->bAllowSpatialization = true;

	//KillBall PowerUp
	killBallOn = false;
	killBallShots = 0;
	//ShotAbsorb PowerUp
	shotAbsorbOn = false;
	shotAbsorbHits = 0;


}

void ARunner::BeginPlay()
{
	//For player characters
	if (!this->isAI)
	{
		ChangeMIntensity(1);
		Super::BeginPlay();
		SetActorHiddenInGame(true);
		DisableInput(GetWorld()->GetFirstPlayerController());
		HUD = Cast<ARunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		HUD->HideHUD(true);
		HUD->SetEnemiesLeft(3);
		GetWorldTimerManager().SetTimer(RunnerStatusHandler, this, &ARunner::ReinstateAll, 12.0f, false);
		InitStateMachines();
		// Begin looping engine audio
		engineAudioComponent->Play();
		// Add to RunnerObserver register
		ARunnerObserver::RegisterRunner(*this);

	}// For AI characters
	else
	{
		Super::BeginPlay();
		HUD = Cast<ARunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		SetActorHiddenInGame(false);
		EnableInput(GetWorld()->GetFirstPlayerController());
		InitStateMachines();
		// Begin looping engine audio
		engineAudioComponent->Play();
		// Add to RunnerObserver register
		ARunnerObserver::RegisterRunner(*this);
	}
}

void ARunner::ReinstateAll()
{
	SetActorHiddenInGame(false);
	EnableInput(GetWorld()->GetFirstPlayerController());
	HUD->HideHUD(false);

	//Commence game timer
	GetWorld()->GetTimerManager().SetTimer(
		GameTimeHandler, this, &ARunner::DecrementGameTime, 1, false);
}

void ARunner::InitStateMachines()
{
	// Aerial SM
	AerialS_Grounded = AerialGroundedState(this);
	AerialS_Air = AerialAirState(this);
	AerialSM.Init(&AerialS_Grounded);

	// Movement SM
	MovementS_Straight = MovementStraightState(this);
	MovementS_Drift = MovementDriftState(this);
	MovementSM.Init(&MovementS_Straight);
}

void ARunner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove from RunnerObserver register
	ARunnerObserver::DeregisterRunner(*this);
}

void ARunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	engineAudioComponent->SetWorldLocation(GetActorLocation());
	
	if (GetController() == GetWorld()->GetFirstPlayerController()) {
		speedUpdateTimer += DeltaTime;
		if (speedUpdateTimer >= 0.2) {
			HUD->SetSpeed(Mover->GetForwardSpeed());
			speedUpdateTimer = 0; 
		}
	}
	
	// Tick state machines
	AerialSM.TickCurrentState(DeltaTime);
	MovementSM.TickCurrentState(DeltaTime);
}

void ARunner::DecrementGameTime()
{
	gameTime--;
	HUD->SetGameTimeRemaining(gameTime);

	gameTime >= 1 ? GetWorld()->GetTimerManager().SetTimer(GameTimeHandler, this, &ARunner::DecrementGameTime, 1, false) : LoseScreen();
}

void ARunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ARunner::SteeringInput);
	PlayerInputComponent->BindAxis("Pitch", this, &ARunner::RotateInput);
	PlayerInputComponent->BindAxis("Accelerate", this, &ARunner::ThrottleInput);
	PlayerInputComponent->BindAxis("Rotate Camera", this, &ARunner::CameraInput);
	PlayerInputComponent->BindAxis("Look Behind", this, &ARunner::HandleLookBehindInput);
	PlayerInputComponent->BindAction("Hop", IE_Pressed, this, &ARunner::Hop);
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &ARunner::LockOn);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARunner::Fire);
	PlayerInputComponent->BindAction("WIN", IE_Pressed, this, &ARunner::WinScreen);
	PlayerInputComponent->BindAction("LOSE", IE_Pressed, this, &ARunner::LoseScreen);
	FInputActionBinding& pause = PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ARunner::Pause); 
	pause.bExecuteWhenPaused = true; 
}

void ARunner::ThrottleInput(float in)
{
	// Determine target gear
	int32 targetGear = 0;
	if (in > 0.f)
		targetGear = 1;
	else if (in < 0.f)
		targetGear = -1;
	Mover->SetTargetGear(targetGear, true);

	// If current speed sign is opposite of target gear, apply brakes
	const float speed = Mover->GetForwardSpeed();
	if ((targetGear == 1 && speed < 0.f) || (targetGear == -1 && speed > 0.f))
		Mover->SetBrakeInput(in);

	// Set engine audio param to Mover's
	engineAudioComponent->SetFloatParameter(FName("EnginePower"), Mover->GetEngineRotationSpeed() / Mover->GetEngineMaxRotationSpeed());

	// Finally, set throttle input
	Mover->SetThrottleInput(in);
}

void ARunner::SteeringInput(float in)
{
	// Steer if the Runner is grounded.
	// Roll over if Runner is in air. 

	if (AerialSM.IsInState(&AerialS_Grounded))
		Mover->SetSteeringInput(in);
	else if (AerialSM.IsInState(&AerialS_Air))
		m_TorqueInput.X = -in * ROTATION_FIX_FORCE;
}

void ARunner::RotateInput(float in)
{
	// Modify pitch, but only if Runner is in air.
	if (AerialSM.IsInState(&AerialS_Air))
		m_TorqueInput.Y = in * ROTATION_FIX_FORCE;
}

// Rotate the camera left/right. Expects a value [-1, 1].
void ARunner::CameraInput(float in)
{
	// Are we locked onto a Runner?
	if (!m_CameraTargetRunner)
	{
		// Set spring arm to face forward
		SpringArm->SetRelativeRotation(FRotator(0.f, in * 90.f, 0.f));

		// Either soft-lock to a runner or aim forward (if GetClosestRunner() returns null)
		const ARunner* softTargetRunner = ARunnerObserver::GetClosestRunner(*this, LOCK_ON_DISTANCE,
			LOCK_ON_FIELD_OF_VIEW, true);
		AimBlaster(softTargetRunner, GetWorld()->GetDeltaSeconds());
	}
	else
	{
		// Set spring arm + blaster to face towards the target runner
		SpringArm->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(SpringArm->GetComponentLocation(), m_CameraTargetRunner->GetActorLocation()));
		FRotator newRelativeRotation = SpringArm->GetRelativeRotation();
		newRelativeRotation.Roll = 0;
		SpringArm->SetRelativeRotation(newRelativeRotation);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%s"), *(SpringArm->GetTargetRotation().ToString())));

		AimBlaster(m_CameraTargetRunner, GetWorld()->GetDeltaSeconds());
	}
}

void ARunner::HandleLookBehindInput(float in)
{
	// Ignore if we are locked on
	if (m_CameraTargetRunner)
		return;

	if (in > LOOK_BEHIND_THRESHOLD) {
		this->LookBehind();
	}
	else if(m_IsLookingBehind) {
		this->ResetCamera();
	}
}

// Rotates the camera to face behind the runner.
void ARunner::LookBehind()
{
	m_IsLookingBehind = true;
	SpringArm->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
}

// Resets the camera to face the default direction.
void ARunner::ResetCamera()
{
	m_IsLookingBehind = false;
	SpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

void ARunner::Hop()
{
	// Only hop if Runner is grounded
	if (AerialSM.IsInState(&AerialS_Grounded)) {
		RootMesh->AddForce(GetActorUpVector() * HOP_FORCE_AMOUNT, FName("b_Root"), true);
		PlaySound(jumpAudioCue);
		//AParticleSpawner::SpawnParticle(ParticleType::Poof, GetActorLocation(), FVector(), 0.3f);
	} 
}

void ARunner::LockOn()
{
	// Currently locked on?
	if (!m_CameraTargetRunner)
	{
		// Target closest visible Runner (may return null)
		m_CameraTargetRunner = ARunnerObserver::GetClosestRunner(*this, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST);
		
		// If found target, start query tick
		if (m_CameraTargetRunner)
			GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &ARunner::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	}
	else
	{
		// Clear currently locked-on Runner
		m_CameraTargetRunner = nullptr;

		// Stop any timers that may be running
		m_LockOnQueryTimer.Invalidate();
	}
}

void ARunner::Fire()
{
	//classes set in Unreal Class Defaults of Runners 
	if (!ProjectileClass) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("===ProjectileClass not initialized - shot failed==="), *GetDebugName(this)));
		return;
	}
	if (!KillBallProjectileClass) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("===KillBallProjectileClass not initialized - shot failed==="), *GetDebugName(this)));
		return;
	}

	auto World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	const auto rot = BlasterBase->GetComponentRotation();
	const auto loc = BlasterBase->GetComponentLocation() + 200.0 * (rot.Vector());	//200 is to account for the length of the barrel

	//check for Kill Ball
	if (killBallOn) {
		ChangeMIntensity(2);
		AKillBallProjectile* projectile = World->SpawnActor<AKillBallProjectile>(KillBallProjectileClass, loc, rot, SpawnParams); // Spawn KillBal
		
		//Fires shot and turn off if we run out of shots
		killBallShots--;
		if (killBallShots <= 0) {
			killBallOn = false;
		}

		if (projectile) {
			//Projectile variables
			projectile->FireOrbInDirection(rot.Vector(), this); //Damage is preset for KillBall
			//Debug
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("You fired your KillBall weapon."), *GetDebugName(this)));
			//Effects
			PlaySound(laserAudioCue);
			AParticleSpawner::SpawnParticle(Poof, BlasterBase->GetComponentLocation(),
				BlasterBase->GetComponentRotation().Vector() * (projectile->ProjectileMovementComponent->InitialSpeed * 0.8f), 0.8f);
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("KILLBALL FAILED"), *GetDebugName(this)));
		}
	}
	else { //Fire a regular orb ball
		ChangeMIntensity(1);
		AOrbProjectile*  projectile = World->SpawnActor<AOrbProjectile>(ProjectileClass, loc, rot, SpawnParams); //Regular orb
		if (projectile) {
			//Projectile variables
			projectile->FireOrbInDirection(rot.Vector(), this);
			projectile->shotDamage = playerDamage; //Set damage of shot to the players damage
			//Debug
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("You fired your regular weapon."), *GetDebugName(this)));
			//Effects
			PlaySound(laserAudioCue);
			AParticleSpawner::SpawnParticle(Poof, BlasterBase->GetComponentLocation(),
				BlasterBase->GetComponentRotation().Vector() * (projectile->ProjectileMovementComponent->InitialSpeed * 0.8f), 0.8f);

		}
	}
}

void ARunner::QueryLockOnEngage()
{
	// Check to see if timer is valid
	if (!m_LockOnQueryTimer.IsValid() || !m_CameraTargetRunner)
		return;

	// Query to see if other runner is visible
	if (ARunnerObserver::IsRunnerVisible(*this, *m_CameraTargetRunner, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST))
	{
		// Loop another engagement query timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &ARunner::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	}
	else
	{
		// Start disengage timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &ARunner::QueryLockOnDisengage, LOCK_ON_NON_VISIBLE_TIME, false);
	}
}

void ARunner::QueryLockOnDisengage()
{
	// Check to see if timer is valid
	if (!m_LockOnQueryTimer.IsValid() || !m_CameraTargetRunner)
		return;

	// Query to see if other runner is visible
	if (ARunnerObserver::IsRunnerVisible(*this, *m_CameraTargetRunner, LOCK_ON_DISTANCE, LOCK_ON_FIELD_OF_VIEW, LOCK_ON_RAYCAST_TEST))
	{
		// Return to engagemenet query timer
		GetWorld()->GetTimerManager().SetTimer(m_LockOnQueryTimer, this, &ARunner::QueryLockOnEngage, LOCK_ON_QUERY_TIME, false);
	}
	else
	{
		// Stop targeting, target has been invisible for too long
		m_CameraTargetRunner = nullptr;
	}
}

void ARunner::AimBlaster(const ARunner* targetRunner, const float deltaTime)
{
	FRotator blasterSlerpedLookAt;

	if (targetRunner)
	{
		//ChangeMIntensity(1);
		// Aim blaster towards target runner
		const FRotator blasterTargetLookAt = UKismetMathLibrary::FindLookAtRotation(
			BlasterBase->GetComponentLocation(), targetRunner->GetActorLocation());
		blasterSlerpedLookAt = UKismetMathLibrary::RLerp(BlasterBase->GetComponentRotation(),
			blasterTargetLookAt, LOCK_ON_BLASTER_RPS * deltaTime, true);

		// Render reticle over target
		HUD->RenderLockOnReticle(targetRunner->GetActorLocation(), false);
	}
	else
	{
		//ChangeMIntensity(0);
		// Aim blaster towards front of runner
		blasterSlerpedLookAt = UKismetMathLibrary::RLerp(BlasterBase->GetComponentRotation(),
			GetActorForwardVector().Rotation(), LOCK_ON_BLASTER_RPS * deltaTime, true);

		// Hide reticle
		HUD->RenderLockOnReticle(FVector(), true);
	}

	// Apply rotation
	BlasterBase->SetWorldRotation(blasterSlerpedLookAt);
}

void ARunner::Pause() {
	ChangeMIntensity(0);
	HUD->Pause(); 
	//AddToHealth(-40);	// Temporarily make it so that pausing hurts you a lot for testing purposes ;)))
}

//Callable function to display on the HUD upon reaching win condition
void ARunner::WinScreen(){
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	ChangeMIntensity(2);
	HUD->YouWin();
}

//Callable function to display on the HUD upon reaching lose condition
void ARunner::LoseScreen() {
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	ChangeMIntensity(0);
	HUD->YouLose();
}

void ARunner::AddToHealth(int newHealth) {
	health += newHealth;
	if (health >= 100) {
		health = 100;
	}
	/* This is when a runner has lost all of its health */
	else if (health <= 0) {
        health = 0;
        if (this->isAI) {  // If an AI just died, destroy the actor and move on, otherwise update player accordingly
			HUD->DecrementEnemiesLeft();
            Destroy();
            return;
        }
        HUD->SetHealth(health);
		//KillBall PowerUp
		killBallOn = false;
		killBallShots = 0;
		//ShotAbsorb PowerUp
		shotAbsorbOn = false;
		shotAbsorbHits = 0;

        HUD->SetDead(true);
		lives--;
		HUD->DecrementLivesLeft();
		if (lives <= 0) {
			LoseScreen();
		}
		FVector CurrentLocation = GetActorLocation();	// We want to keep track of where the runner was when it died

		
		/* This code will make it seem to disappear. Source: https://forums.unrealengine.com/t/disable-an-actor/4738/22 */

		/* Stop its movement and teleport it up to prevent collisions first */
		Mover->Deactivate();
		const FVector higher = { CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + 100 };
		const FRotator newOrientation = { 0, 0, 0 };
		TeleportTo(higher, newOrientation);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		/* If the player just died, they shouldn't be able to move until they respawn */
        DisableInput(GetWorld()->GetFirstPlayerController());
														
		/* This code will make it wait three second to respawn. Source: https://www.codegrepper.com/code-examples/cpp/unreal+engine+delay+c%2B%2B */
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Respawning in 3 seconds..."), *GetDebugName(this)));
		if (HUD->getLives()> 0 && HUD->getEnemiesLeft() > 0) {
			HUD->SetTimeLeft(3);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("2 seconds left to respawn..."), *GetDebugName(this)));
					HUD->SetTimeLeft(2);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("1 second left to respawn..."), *GetDebugName(this)));
							HUD->SetTimeLeft(1);
							GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
								{
									Respawn();	// After three seconds, the runner respawns at one of the four corners
									EnableInput(GetWorld()->GetFirstPlayerController());
									Mover->Activate();
								}, 1, false
							);
						}, 1, false
					);
				}, 1, false
			);
		}
    } else if (!this->isAI) {
        HUD->SetHealth(health);
	}
}

/* Tries to move the runner to one of the four corners of the map. If it fails 5 times, it will not teleport it because something went wrong and an infinite loop is likely */
void ARunner::Respawn() {
	FVector CurrentLocation = GetActorLocation();	// We want to keep track of where the runner was when it died
	int respawnAttemptCounter = 0;	// Keep track of how many times respawning has failed. If it's failed more than 5 times, it's probably stuck in an infinite loop so just give up
	while (GetActorLocation() == CurrentLocation) {	// Continue attempting to relocate the runner until it has been moved to a respawn point
		if (respawnAttemptCounter > 5) {
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Respawning has been attempted multiple times without success, aborting respawn attempts to prevent an infinite loop!"), *GetDebugName(this)));
			return;
		}
		int selectedPoint = rand() % 4;	// Grab a random number 0 - 3, each number represents one of the map's corners
		FVector newLocation = { (float)(4700 * (pow(-1, ((selectedPoint + 1) / 2)))) , (float)(3600 * (pow(-1, (selectedPoint / 2)))), (float)(0) };	// Use math to pick the location of the chosen corner
		FRotator newOrientation = { (float)(0), (float)(225 + 90 * selectedPoint) , (float)(0) };	// Use math to rotate the runner the amount that is appropriate for the selected corner
		TeleportTo(newLocation, newOrientation);	// Try to teleport to the calculated corner with the calculated rotation
		respawnAttemptCounter++;
	}
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	health = 100;	// Reset to full health
    HUD->SetHealth(health);
	if (!this->isAI) HUD->SetDead(false);
}

void ARunner::AddToScore(int newScore) {
	if (GetController() == GetWorld()->GetFirstPlayerController()) {
		score += newScore;
		HUD->AddToScore(newScore);
	} 
	//WIN CONDITION (Adjust for testing purposes)
	if (score >= 5000){
		HUD->YouWin();
	}
}
//PowerUps can call this to change player damage
void ARunner::AddToDamage(int addedDamage) {
	//ChangeMIntensity(1);
	playerDamage += addedDamage;
}

//PowerUps call this to use ShotAbsorb and add hits
void ARunner::obstainShotAbsorbPower(int hits) {
	ChangeMIntensity(2);
	shotAbsorbOn = true;
	shotAbsorbHits = shotAbsorbHits + hits; //This powerup stacks
}

//PowerUps call this to use KillBall and add hits
void ARunner::obstainKillBallPower(int hits) {
	ChangeMIntensity(2);
	killBallOn = true;
	killBallShots = killBallShots + hits; //This powerup indeed stacks
}

//Orbs call this function when they hit the runner
//Holds the needed steps to deal damage based on current powerups
void ARunner::hitMe(int damage) {
	
	//Powerups may change or negate the damage done to the runner

	//Invisibity first (Time constrainted)
	//Shield second (Damage constrainted and technically would be outside the runner to be hit first)
	if (shotAbsorbOn) { //ShotAbsorb third (Not timed or damage constrainted but is shot constrainted instead) 
		shotAbsorbHits = shotAbsorbHits - 1;
		if (shotAbsorbHits <= 0) {
			shotAbsorbOn = false;
		}
	} else { //No power ups prevent the Shot from hitting runner
		AddToHealth(damage); //Damage should be negative when passed into function
	}

} 

bool ARunner::IsGrounded()
{
	// Buffer for raycast hit result
	FHitResult hitResult;

	// Calculate world space positions of start and end of cast
	FVector start = RootMesh->GetBoneLocation("b_Root", EBoneSpaces::WorldSpace);
	FVector end = start + (-GetActorUpVector() * GROUNDING_RAYCAST_DIST);

	// Check if raycast hits something
	// TODO: Filter what types of objects are considered to be the 'floor'
	return GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_WorldStatic, GROUNDING_COLLISION_PARAMS);
}

void ARunner::FixRotation()
{
	// The rotation's pitch and roll ideally need to be in a certain threshold to keep the game enjoyable
	// If outside of that threshold, apply rotational force to the RootMesh in the opposite direction

	// Get current runner rotation
	const FRotator runnerRotation = GetActorRotation();

	// Determine orientation of the runner, and write to Torque input if beyond threshold
	// Note: This overrides any possible input made by the player.

	// Fix pitch if needed
	if (runnerRotation.Pitch < -ROTATION_FIX_THRESHOLD)
		m_TorqueInput.Y = -ROTATION_FIX_FORCE * 1.5f;	// Do backflip
	else if (runnerRotation.Pitch > ROTATION_FIX_THRESHOLD)
		m_TorqueInput.Y = ROTATION_FIX_FORCE * 1.5f;	// Do frontflip

	// Fix roll if needed
	if (runnerRotation.Roll < -ROTATION_FIX_THRESHOLD)
		m_TorqueInput.X = -ROTATION_FIX_FORCE * 1.5f;	// Do Left-Cartwheel
	else if (runnerRotation.Roll > ROTATION_FIX_THRESHOLD)
		m_TorqueInput.X = ROTATION_FIX_FORCE * 1.5f;	// Do Right-Cartwheel

	// Transform m_TorqueInput to runner's local space
	FVector runnerTorque = runnerRotation.RotateVector(m_TorqueInput);

	// Finally, apply combined input + fixed torque to the runner
	RootMesh->AddTorqueInRadians(runnerTorque * GetWorld()->DeltaTimeSeconds, FName("b_Root"), true);
}

// Audio -------------------------------------------------------------------------------------------------------

void ARunner::PlaySound(USoundCue* cue) {
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), cue, GetActorLocation(), FRotator::ZeroRotator, 1.0, 1.0, 0.0f, attenuation, nullptr, this);
}


//void ARunner::ChangeMIntensity(int intensity) {
	
//}

// State machine ------------------------------------------------------------------------------------------------


// Aerial States ------------------------------------------

void ARunner::AerialGroundedState::OnEnter()
{
	GetRunner()->PlaySound(GetRunner()->thudAudioCue); 

	AParticleSpawner::SpawnParticle(ParticleType::Poof, GetRunner()->GetActorLocation(), FVector(), 0.3f);
}

void ARunner::AerialGroundedState::OnExit()
{

}

void ARunner::AerialGroundedState::Tick(float DeltaTime)
{
	// If runner down-raycast fails, transition to air state
	if (!GetRunner()->IsGrounded())
	{
		GetRunner()->AerialSM.TransitionState(&(GetRunner()->AerialS_Air));
		return;
	}

	// TODO: Charge hopping ability?
}

void ARunner::AerialAirState::OnEnter()
{
	// Disable drifting if active
	if (GetRunner()->MovementSM.IsInState(&(GetRunner()->MovementS_Drift)))
		GetRunner()->MovementSM.TransitionState(&(GetRunner()->MovementS_Straight));
}

void ARunner::AerialAirState::OnExit()
{

}

void ARunner::AerialAirState::Tick(float DeltaTime)
{
	// If runner down-raycast succeeds, transition to grounded state
	if (GetRunner()->IsGrounded())
	{
		GetRunner()->AerialSM.TransitionState(&(GetRunner()->AerialS_Grounded));
		return;
	}

	// Fix runner rotation if it is outside the threshold
	GetRunner()->FixRotation();
}

// Movement States ------------------------------------------

void ARunner::MovementStraightState::OnEnter()
{

}

void ARunner::MovementStraightState::OnExit()
{

}

void ARunner::MovementStraightState::Tick(float DeltaTime)
{

}

void ARunner::MovementDriftState::OnEnter()
{
	// TODO: Enable drift movement
}

void ARunner::MovementDriftState::OnExit()
{
	// TODO: Disable drift movement / boost?
}

void ARunner::MovementDriftState::Tick(float DeltaTime)
{
	// TODO: Simulate drift, rotate kart at root bone?
}

// Get Runner HUD ------------------------------------------

ARunnerHUD* ARunner::GetRunnerHUD() { return HUD; }
