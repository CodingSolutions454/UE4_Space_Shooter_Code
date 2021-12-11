// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Enemy2.h"
#include "SpaceShooterGameMode.h"
#include "SpaceShooterCharacter.h"
#include "SpaceShooter.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "TrackingLaser1.h"
#include "Score_50.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAI_Bot_Enemy2::AAI_Bot_Enemy2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		SetGenericTeamId(FGenericTeamId(5));
	}
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	//Construct Static Mesh Component
	UFOShip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOShip"));
	UFOShip->SetupAttachment(RootComponent);

	//Construct Static Mesh Component
	UFOLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOLight"));
	UFOLight->SetupAttachment(RootComponent);
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	//Bools set here
	bDead = false;
	Detected = false;

	//Position laser cannon is fired from
	EnemyLaserCannonOffset = FVector(100.0f, 0.0f, 10.0f);

	//float values
	EnemyShield2 = 100.0f;
	FireRate = 6.0f;


	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .25f; // 4 times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

void AAI_Bot_Enemy2::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{

	//If this actor has collision with laser1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Laser1")) {

		EnemyShield2 -= 26.0f;

		if (EnemyShield2 > 100.0f)
			EnemyShield2 = 100.0f;

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

	//If this actor has collision with Rocket1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Missile1")) {

		EnemyShield2 -= 200.0f;

		if (EnemyShield2 > 100.0f)
			EnemyShield2 = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());

		}
	}
}

// OnPawn detected, cast to main charcater and fire a prjectile towrads the main character. Play Sound and create animation
void AAI_Bot_Enemy2::SeePawn(APawn* Pawn)
{
	if (PawnSensing->OnSeePawn, true) {

		ASpaceShooterCharacter* Actor = Cast<ASpaceShooterCharacter>(PawnSensing);

		// try and fire a TrackingLaser1
		if (ATrackingLaser1class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				{
					const FRotator SpawnRotation = GetActorRotation();

					const FVector SpawnLocation = ((EnemyLaserCannon != nullptr) ? EnemyLaserCannon->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(EnemyLaserCannonOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the Laser2 at the laser cannon bone
					World->SpawnActor<ATrackingLaser1>(ATrackingLaser1class, SpawnLocation, SpawnRotation, ActorSpawnParams);

					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAI_Bot_Enemy2::ShotTimerExpired, FireRate);

					// Plays animation for whne laser is being fired.
					GetMesh()->SetAnimation(TrackingLaser1Fire);
					GetMesh()->PlayAnimation(TrackingLaser1Fire, false);

					// try and play the sound if specified
					if (TrackingLaser1Sound != NULL)
					{
						UGameplayStatics::PlaySoundAtLocation(this, TrackingLaser1Sound, GetActorLocation());
					}

					bCanFire = false;
				
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AAI_Bot_Enemy2::BeginPlay()
{
	Super::BeginPlay();

	// Get capsule component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy2::OnBeginOverLap);

	PawnSensing->OnSeePawn.AddDynamic(this, &AAI_Bot_Enemy2::SeePawn);
}

// Called every frame
void AAI_Bot_Enemy2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// If EnemyShield is less than 0. Simulate physics, play animation, play sound then destroy the capsule to avoid collisions
	if (EnemyShield2 < 1) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			Controller->Destroy();

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Destroy();

			// When this actor is destroyed. Create This particle effect at location.
			UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionSystem, GetActorTransform());
			Explosion->SetRelativeScale3D(FVector(5.f));

			// try and fire Score_50
			if (AScore_50class != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					{
						const FRotator SpawnRotation = GetActorRotation();

						const FVector SpawnLocation = ((EnemyLaserCannon != nullptr) ? EnemyLaserCannon->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(EnemyLaserCannonOffset);
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
UParticleSystemComponent* AAI_Bot_Enemy2::CreateExplosion(UParticleSystem* Explosion)
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
UParticleSystemComponent* AAI_Bot_Enemy2::CreateSpark(UParticleSystem* Spark)
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
void AAI_Bot_Enemy2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Generic team ID
FGenericTeamId AAI_Bot_Enemy2::GetGenericTeamId() const
{
	return FGenericTeamId(10);
}

// Attach the eyes to the bone stated in quotation
void AAI_Bot_Enemy2::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation("head");
	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}

// If we are firing set this delay
void AAI_Bot_Enemy2::ShotTimerExpired()
{
	bCanFire = true;
}
