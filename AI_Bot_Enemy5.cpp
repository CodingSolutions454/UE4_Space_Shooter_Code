// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Enemy5.h"
#include "SpaceShooterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GenericTeamAgentInterface.h"
#include "SpaceShooter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values
AAI_Bot_Enemy5::AAI_Bot_Enemy5(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		SetGenericTeamId(FGenericTeamId(5));
	}
	
	//Construct Static Mesh Component
	UFOShip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOShip"));
	UFOShip->SetupAttachment(RootComponent);

	//Construct Static Mesh Component
	UFOLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOLight"));
	UFOLight->SetupAttachment(RootComponent);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

	//Bools set here
	bDead = false;
	Detected = false;

	//float values
	EnemyShield5 = 100.0f;
	FireRate = 6.0f;

	//Position laser cannon is fired from
	laser_cannonOffset = FVector(100.0f, 0.0f, 10.0f);

	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .25f; // 4 times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

void AAI_Bot_Enemy5::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{

	//If this actor has collision with laser1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Laser1")) {

		EnemyShield5 -= 16.0f;

		if (EnemyShield5 > 100.0f)
			EnemyShield5 = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision.Create a this effect.
		UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
		Spark->SetRelativeScale3D(FVector(2.f));
	}

	//If this actor has collision with Rocket1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Missile1")) {

		EnemyShield5 -= 50.0f;

		if (EnemyShield5 > 100.0f)
			EnemyShield5 = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());

		}
	}
}

// OnPawn detected, cast to main charcater and fire a projectile towrads the main character. Play Sound and create animation
void AAI_Bot_Enemy5::SeePawn(APawn* Pawn)
{
	if (PawnSensing->OnSeePawn, true) {

		ASpaceShooterCharacter* Actor = Cast<ASpaceShooterCharacter>(PawnSensing);

		// try and fire a Laser5
		if (ALaser5class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				{
					const FRotator SpawnRotation = GetActorRotation();

					const FVector SpawnLocation = ((RightLaserTurrent != nullptr) ? RightLaserTurrent->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(RightLaserTurrentOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the Laser5 at the laser cannon bone
					World->SpawnActor<ALaser5>(ALaser5class, SpawnLocation, SpawnRotation, ActorSpawnParams);

					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAI_Bot_Enemy5::ShotTimerExpired, FireRate);

					// try and play the sound if specified
					if (Laser5Sound != NULL)
					{
						UGameplayStatics::PlaySoundAtLocation(this, Laser5Sound, GetActorLocation());
					}
				}
			}
		}
	}
}

//Called when the game starts or when spawned
void AAI_Bot_Enemy5::BeginPlay()
{
	Super::BeginPlay();

	// Get capsule component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy5::OnBeginOverLap);

	PawnSensing->OnSeePawn.AddDynamic(this, &AAI_Bot_Enemy5::SeePawn);
}

// Called every frame
void AAI_Bot_Enemy5::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// If EnemyShield is less than 0. Simulate physics, play animation, play sound then destroy the capsule to avoid collisions
	if (EnemyShield5 < 1) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			Controller->Destroy();

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Destroy();

			// When this actor is destroyed. Create This particle effect at location.
			UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionSystem, GetActorTransform());
			Explosion->SetRelativeScale3D(FVector(8.f));

			// try and fire a Score_50
			if (AScore_50class != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					{
						const FRotator SpawnRotation = GetActorRotation();

						const FVector SpawnLocation = ((RightLaserTurrent != nullptr) ? RightLaserTurrent->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(RightLaserTurrentOffset);
						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the Rocket1 at the Rocket cannon bone
						World->SpawnActor<AScore_50>(AScore_50class, SpawnLocation, SpawnRotation, ActorSpawnParams);

						// try and play the sound if specified
						if (ExplosionSound != NULL)
						{
							UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
						}
					}
				}
			}
		}
	}
}

// Get this actors location and create the explosion there
UParticleSystemComponent* AAI_Bot_Enemy5::CreateExplosion(UParticleSystem* Explosion)
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
UParticleSystemComponent* AAI_Bot_Enemy5::CreateSpark(UParticleSystem* Spark)
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

// Called to bind functionality to input
void AAI_Bot_Enemy5::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Generic team ID
FGenericTeamId AAI_Bot_Enemy5::GetGenericTeamId() const
{
	return FGenericTeamId(10);
}

// Attach the eyes to the bone stated in quotation
void AAI_Bot_Enemy5::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation("head");
	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}

// If we are firing set this delay
void AAI_Bot_Enemy5::ShotTimerExpired()
{
	bCanFire = true;
}
