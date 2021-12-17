// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceShooterCharacter.h"
#include "SpaceShooterGameMode.h"
#include "SpaceShooter.h"
#include "AI_Bot_Enemy1.h"
#include "AI_Bot_Enemy2.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Left_ShipLight.h"
#include "Laser1.h"
#include "Score_50.h"
#include "Missile.h"
#include "SMainMenuWidget.h"
#include "TrackingLaser1.h"
#include "MySaveGame.h"
#include "AI_Bot_Enemy1.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

ASpaceShooterCharacter::ASpaceShooterCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is the team my character is in and its different to the enemy team. It so I am identified as an enemy my the enemy team.
	{
		// Team 10
		SetGenericTeamId(FGenericTeamId(10));
	}

	//Construct Static Mesh Component
	SpaceShip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceShip"));
	SpaceShip->SetupAttachment(RootComponent);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = true;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1800.f;
	CameraBoom->SocketOffset = FVector(-600.f, 650.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(270.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.3f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 3000.f;
	GetCharacterMovement()->MaxFlySpeed = 3000.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true; //two events are mutually exclusive

	// Widget float and other float values													  
	Shield = 100,0.f;
	Laser = 100,0.f;
	Rockets = 100, 0.f;
	FireRate = 2.0f;
	DashRate = 2.0f;
	DashCharge = 100.0f;

	//Position laser cannon is fired from
	laser_cannonOffset = FVector(100.0f, 0.0f, 10.0f);

	//Position laser cannon is fired from
	Rocket_cannonOffset = FVector(100.0f, 0.0f, 10.0f);
	
	//Position laser cannon is fired from
	Left_Light_SocketOffset = FVector(100.0f, 0.0f, 10.0f);


	//Bools here
	bDead = false;
	bCanFire = true;
	bCanDash = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// If we are firing set this delay
void ASpaceShooterCharacter::ShotTimerExpired()
{
	bCanFire = true;
}

// Dash cool down
void ASpaceShooterCharacter::DashTimerExpired()
{
	bCanDash = true;
}

// This characters collision and condition if we hit something
void ASpaceShooterCharacter::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	// If this player collides with this portel. Open this level.
	if (OtherActor->ActorHasTag("Portal_1")) {
		
		UGameplayStatics::OpenLevel(this, "Level_2");

	}
	
	// If this player collides with this portel. Open this level.
	if (OtherActor->ActorHasTag("Portal_2")) {

		UGameplayStatics::OpenLevel(this, "Level_3");
	}
	
	// If this actor has collision with laser2 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Laser2")) {

		Shield -= 20.0f;

		if (Shield > 100.0f)
			Shield = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision. Create a this effect.
		UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
		Spark->SetRelativeScale3D(FVector(2.f));
	}

	// If this actor has collision with laser4 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Laser4")) {

		Shield -= 20.0f;

		if (Shield > 100.0f)
			Shield = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision. Create a this effect.
		UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
		Spark->SetRelativeScale3D(FVector(2.f));
	}

	// If this actor has collision with laser5 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Laser5")) {

		Shield -= 20.0f;

		if (Shield > 100.0f)
			Shield = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision. Create a this effect.
		UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
		Spark->SetRelativeScale3D(FVector(2.f));
	}
	
	// If this actor has collision with Asteroid_Projectile_1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Asteroid_Projectile_1")) {

		Shield -= 35.0f;

		if (Shield > 100.0f)
			Shield = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision. Create a this effect.
		UParticleSystemComponent* Smoke = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeSystem, GetActorTransform());
		Smoke->SetRelativeScale3D(FVector(2.f));
	}
	// If this actor has collision with laser2 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("AI_Bot_Enemy4")) {

		Shield -= 10.0f;

		if (Shield > 100.0f)
			Shield = 100.0f;

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision. Create a this effect.
		UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
		Spark->SetRelativeScale3D(FVector(2.f));
	}
	// If we defeat AI_Bot_Enemy1. This enemy will create the projectile that travels towards us and on collision awards 50 score points then destroys itself
	if (OtherActor->ActorHasTag("Score_50")) {

		Score += 50.0f;

		OtherActor->Destroy();
	}
	// If this actor has collision with TrackingLaser1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("TrackingLaser1")) {

		Shield -= 15.0f;

		if (Shield > 100.0f)
			Shield = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision. Create a this effect.
		UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
		Spark->SetRelativeScale3D(FVector(2.f));
	}
}

void ASpaceShooterCharacter::RestartGame()
{
	// If health is 0. Restart game and open world
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}


// Get this actors location and create the explosion there
UParticleSystemComponent* ASpaceShooterCharacter::CreateExplosion(UParticleSystem* Explosion)
{
	{
		class UParticleSystemComponent* retVal = NULL;

		if (Explosion)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, Explosion, myPos, myRot, true);
			}
		}

		return retVal;
	}
}

// Get this actors location and create the spark there
UParticleSystemComponent* ASpaceShooterCharacter::CreateSpark(UParticleSystem* Spark)
{
	{
		class UParticleSystemComponent* retVal = NULL;

		if (Spark)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, Spark, myPos, myRot, true);
			}
		}

		return retVal;
	}
}

// Get this actors location and create the DashStream there
UParticleSystemComponent* ASpaceShooterCharacter::CreateDashStream(UParticleSystem* DashStream)
{

	{
		class UParticleSystemComponent* retVal = NULL;

		if (DashStream)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, DashStream, myPos, myRot, true);
			}
		}

		return retVal;
	}

}

// Get this actors location and create the CreateSmoke there
UParticleSystemComponent* ASpaceShooterCharacter::CreateSmoke(UParticleSystem* Smoke)
{
	{
		class UParticleSystemComponent* retVal = NULL;

		if (Smoke)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, Smoke, myPos, myRot, true);
			}
		}

		return retVal;
	}
}

// Called when the game starts or when spawned
void ASpaceShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Get capsule component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShooterCharacter::OnBeginOverLap);
}



void ASpaceShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASpaceShooterCharacter::Dash(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	// Set up to launch this character. Launch happens when we dash and can be found in the dash inputs
}

void ASpaceShooterCharacter::MoveRight(float Value)
{
	//add movement in that direction
      AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void ASpaceShooterCharacter::MoveUpDown(float Value)
{
	//add movement in that direction
    AddMovementInput(FVector(1.f, 0.f, 1.f), Value);
}

void ASpaceShooterCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ASpaceShooterCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	//Stop Jumping
	StopJumping();
}

void ASpaceShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
// set up gameplay key bindings
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ASpaceShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAction("Save", IE_Pressed, this, &ASpaceShooterCharacter::SaveGame);
	PlayerInputComponent->BindAction("Load", IE_Pressed, this, &ASpaceShooterCharacter::LoadGame);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASpaceShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASpaceShooterCharacter::StopJumping);
	PlayerInputComponent->BindAction("DashBack", IE_Released, this, &ASpaceShooterCharacter::DashBack);
	PlayerInputComponent->BindAction("DashForward", IE_Released, this, &ASpaceShooterCharacter::DashForward);
	PlayerInputComponent->BindAction("DashLeft", IE_Released, this, &ASpaceShooterCharacter::DashLeft);
	PlayerInputComponent->BindAction("DashRight", IE_Released, this, &ASpaceShooterCharacter::DashRight);
	InputComponent->BindAction("LightsOn", IE_Pressed, this, &ASpaceShooterCharacter::LightsOn);
	InputComponent->BindAction("LigtsOff", IE_Pressed, this, &ASpaceShooterCharacter::LightsOff);
	
	
	InputComponent->BindAction("FireLaser1", IE_Pressed, this, &ASpaceShooterCharacter::FirePrimary);
	InputComponent->BindAction("FireRocket1", IE_Pressed, this, &ASpaceShooterCharacter::FireSecondary);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASpaceShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUpDown"), this, &ASpaceShooterCharacter::MoveUpDown);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASpaceShooterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASpaceShooterCharacter::TouchStopped);
}

// Called every frame
void ASpaceShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetCursorDir();//the method will be called every frame.

	// Recharges shield after taking damage
	Shield += Shield_Treshold =.1;

	if (Shield > 100) 
	{
		Shield -= Shield_Treshold = .1;
	}
	
	// Recharge laser over time
	Laser += Laser_Treshold = .1;

	// If the laser is more than a 100. Laser will stop recharging
	if (Laser > 100) 
	{
		Laser -= Laser_Treshold = .1;
	}
	if (Laser < 1)
	{
		Laser += Laser_Treshold = .1;
	}
	// Recharge rockets over time
	Rockets += Rockets_Treshold = .1;

	// If the laser is more than a 100. Laser will stop recharging
	if (Rockets > 100)
	{
		Rockets -= Rockets_Treshold = .1;
	}
	if (Rockets < 1)
	{
		Rockets += Rockets_Treshold = .1;
	}
	
	// Recharge dash over time
	DashCharge += DashCharge_Treshold = .5;
	
	// If the dash is more than a 100. Laser will stop recharging
	if (DashCharge > 100)
	{
		DashCharge -= DashCharge_Treshold = .5;
	}
	if (DashCharge < 1)
	{
		DashCharge += DashCharge_Treshold = .5;
	}
	
	// 1f shield is less than 0. Simulate physics, play animation and restart game and destroy this character
	if (Shield < 0) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			// When this actor is destroyed. Create This particle effect at location.
			UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionSystem, GetActorTransform());
			Explosion->SetRelativeScale3D(FVector(5.f));

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ASpaceShooterCharacter::RestartGame, 1.0f, false);
		}
	}

	if (bCanDash == false)
	{
		
		// if this actor has a does the dash. Create a this effect.
		UParticleSystemComponent* DashStream = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DashStreamSystem, GetActorTransform());
		DashStream->SetRelativeScale3D(FVector(2.f));
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

void ASpaceShooterCharacter::SetCursorDir() 
{
	//FVector CursorWorldLocation, CursorWorldDirection;
	//GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	//=====================================================================//

	//gets location of the actor in the world
	FVector CurrLoc = this->GetActorLocation();

	// Get the mouse cursor in world and face mouse cursor
	FHitResult hitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult);
	FVector hitLoc = hitResult.Location;

	//geting the original rotation of the acter;
	FRotator newRot = this->GetActorRotation();
	//Using atan2 function to get the degree between our character location and cursor location
	// aka how much we want the character to rotate
	float newYaw = (hitLoc - CurrLoc).Rotation().Yaw;;
	//Using that degree as the Yaw(rotating around Z axis) of our Frotator
	newRot.Yaw = newYaw;

	//in the end, we set it;
	this->GetController()->SetControlRotation(newRot);
}

// Toggle lights on and snap them to the chosen socket
void ASpaceShooterCharacter::LightsOn()
{
	FActorSpawnParameters SpawnParems;
	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Left_ShipLightclass)
	{
		FTransform Left_ShipLightTransform;

		Left_ShipLightTransform.SetLocation(FVector::ZeroVector);
		Left_ShipLightTransform.SetRotation(FQuat(FRotator::ZeroRotator));


		Left_ShipLight = GetWorld()->SpawnActor<ALeft_ShipLight>(Left_ShipLightclass, Left_ShipLightTransform, SpawnParems);
		if (Left_ShipLight)
		{

			Left_ShipLight->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Left_Light_Socket"));


		}
	}


	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ASpaceShooterCharacter::LightsOff()
{
	if (RemoveLight) 
	{

		GetMesh()->SetAnimation(RemoveLight);
		GetMesh()->PlayAnimation(RemoveLight, false);

		Left_ShipLight->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hiddenSocket"));



	}
}

void ASpaceShooterCharacter::SaveGame()
{
	//Create an instance of our savegame class
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	//Set the save game instance location equal to players location
	SaveGameInstance->PlayerLocation = this->GetActorLocation();
	//Save the Gameinstance
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("My Slot"), 0);
	//Log message to show we have saved game
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Saved"));
}

void ASpaceShooterCharacter::LoadGame()
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	//Load the saved game in to our savegameinstance varible
	SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("My Slot", 0));
	//Set the players position from the saved game file
	this->SetActorLocation(SaveGameInstance->PlayerLocation);
	//Log message to show we have loaded game
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Loaded"));
}

void ASpaceShooterCharacter::FirePrimary()
{
	// try and fire a Rocket1
	if (ALaser1class != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{

			{
				const FRotator SpawnRotation = GetActorRotation();

				const FVector SpawnLocation = ((laser_cannon != nullptr) ? laser_cannon->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(laser_cannonOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the Rocket1 at the Rocket cannon bone
				World->SpawnActor<ALaser1>(ALaser1class, SpawnLocation, SpawnRotation, ActorSpawnParams);

				// Plays animation for whne Rocket is being fired.
				GetMesh()->SetAnimation(Laser1Fire);
				GetMesh()->PlayAnimation(Laser1Fire, false);

				// Lose 2 laser1 amount for every shot fired
				Laser -= 2.00f;

				// if the laser1 is fired and the value is under 1. Subtract 10 from the shield for every shot fired
				if (Laser < 1)
				{
					Shield -= Shield_Treshold = 4;

				}
				
				// try and play the sound if specified
				if (Laser1Sound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, Laser1Sound, GetActorLocation());
				}
			}
		}
	}
}

void ASpaceShooterCharacter::FireSecondary()
{
	if (bCanFire == true)
	{
		// try and fire a Rocket1
		if (AMissileclass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				{
					const FRotator SpawnRotation = GetActorRotation();

					const FVector SpawnLocation = ((Rocket_cannon != nullptr) ? Rocket_cannon->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(Rocket_cannonOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the Rocket1 at the Rocket cannon bone
					World->SpawnActor<AMissile>(AMissileclass, SpawnLocation, SpawnRotation, ActorSpawnParams);

					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceShooterCharacter::ShotTimerExpired, FireRate);

					// Plays animation for whne Rocket is being fired.
					GetMesh()->SetAnimation(Missile1Fire);
					GetMesh()->PlayAnimation(Missile1Fire, false);

					// Lose 33 Rocket amount for every shot fired
					Rockets -= 43.0f;

					// if the Rocket is fired and the value is under 1. Subtract 10 from the shield for every shot fired
					if (Rockets < 1)
					{
						Shield -= Shield_Treshold = 20;

					}

					// try and play the sound if specified
					if (MissileSound != NULL)
					{
						UGameplayStatics::PlaySoundAtLocation(this, MissileSound, GetActorLocation());
					}

					// timer for how often we can fire
					bCanFire = false;
				}
			}
		}
	}
}

// Launch ability for this character. Character will be launched in the direction of the vector and the dash charge time is called
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ASpaceShooterCharacter::DashBack()
{
	if (bCanDash == true)
	{
		UWorld* const World = GetWorld();
		if (World != NULL) 
		{
			FVector launchVelocity = FVector(0.0f, 10000.0f, 0.0f);

			LaunchCharacter(launchVelocity, false, true);

			World->GetTimerManager().SetTimer(TimerHandle_DashTimerExpired, this, &ASpaceShooterCharacter::DashTimerExpired, DashRate);

			bCanDash = false;

			DashCharge -= 100.0f;

			// try and play the sound if specified
			if (DashSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}

			// if this actor has a collision. Create a this effect.
			UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
			Spark->SetRelativeScale3D(FVector(2.f));
		}
	}
}

void ASpaceShooterCharacter::DashForward()
{
	if (bCanDash == true) 
	{
		UWorld* const World = GetWorld();
		if (World != NULL) 
		{
			FVector launchVelocity = FVector(0.0f, -10000.0f, 0.0f);

			LaunchCharacter(launchVelocity, false, true);

			World->GetTimerManager().SetTimer(TimerHandle_DashTimerExpired, this, &ASpaceShooterCharacter::DashTimerExpired, DashRate);

			bCanDash = false;

			DashCharge -= 100.0f;

			// try and play the sound if specified
			if (DashSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}

			// if this actor has a collision. Create a this effect.
			UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
			Spark->SetRelativeScale3D(FVector(2.f));
		}
	}
}

void ASpaceShooterCharacter::DashLeft()
{
	if (bCanDash == true) 
	{
		UWorld* const World = GetWorld();
		if (World != NULL) 
		{

			FVector launchVelocity = FVector(-10000.0f, .0f, 0.0f);

			LaunchCharacter(launchVelocity, false, true);

			World->GetTimerManager().SetTimer(TimerHandle_DashTimerExpired, this, &ASpaceShooterCharacter::DashTimerExpired, DashRate);

			bCanDash = false;

			DashCharge -= 100.0f;

			// try and play the sound if specified
			if (DashSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}

			// if this actor has a collision. Create a this effect.
			UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
			Spark->SetRelativeScale3D(FVector(2.f));
		}
	}
}

void ASpaceShooterCharacter::DashRight()
{
	if (bCanDash == true) 
	{
		UWorld* const World = GetWorld();
		if (World != NULL) 
		{


			FVector launchVelocity = FVector(10000.0f, .0f, 0.0f);

			LaunchCharacter(launchVelocity, false, true);

			World->GetTimerManager().SetTimer(TimerHandle_DashTimerExpired, this, &ASpaceShooterCharacter::DashTimerExpired, DashRate);

			bCanDash = false;

			DashCharge -= 100.0f;

			// try and play the sound if specified
			if (DashSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}

			// if this actor has a collision. Create a this effect.
			UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
			Spark->SetRelativeScale3D(FVector(2.f));
        }
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic team ID
FGenericTeamId ASpaceShooterCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(10);
}















