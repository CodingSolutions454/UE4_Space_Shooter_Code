// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Bot_Enemy4.h"
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
#include "Laser1.h"
#include "Laser3.h"
#include "Laser4.h"
#include "Score_50.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAI_Bot_Enemy4::AAI_Bot_Enemy4(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		SetGenericTeamId(FGenericTeamId(5));
	}
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);

	// Create a right Senser for this actor
	FrontSensor = CreateDefaultSubobject<USphereComponent>(TEXT("FrontSensor"));
	FrontSensor->SetupAttachment(GetRootComponent());
	FrontSensor->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy4::OnProjectileEnterFrontSensor);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 2500.0f;

	//Bools set here
	bDead = false;
	Detected = false;

	//float values
	EnemyShield4 = 100.0f;
	FireRate = 6.0f;

	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;

	//Position laser cannon is fired from
	laser_cannonOffset = FVector(100.0f, 0.0f, 10.0f);

	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .25f; // 4 times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

void AAI_Bot_Enemy4::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{

	//If this actor has collision with laser1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Laser1")) {

		EnemyShield4 -= 200.0f;

		if (EnemyShield4 > 100.0f)
			EnemyShield4 = 100.0f;

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

		EnemyShield4 -= 200.0f;

		if (EnemyShield4 > 100.0f)
			EnemyShield4 = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());

		}
	}
}


void AAI_Bot_Enemy4::SeePawn(APawn* Pawn)
{
	// This logic is not needed here as this enemy does not fire projectils. 
}

void AAI_Bot_Enemy4::Dodge(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	// Lanch Character logic is used in the left and right sensor. If we hit something
}

//Called when the game starts or when spawned
void AAI_Bot_Enemy4::BeginPlay()
{
	Super::BeginPlay();

	// Get capsule component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy4::OnBeginOverLap);

	PawnSensing->OnSeePawn.AddDynamic(this, &AAI_Bot_Enemy4::SeePawn);
}

// Called every frame
void AAI_Bot_Enemy4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// If EnemyShield is less than 0. Simulate physics, play animation, play sound then destroy the capsule to avoid collisions
	if (EnemyShield4 < 1) {

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

						const FVector SpawnLocation = ((laser_cannon != nullptr) ? laser_cannon->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(laser_cannonOffset);
						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the Rocket1 at the Rocket cannon bone
						World->SpawnActor<AScore_50>(AScore_50class, SpawnLocation, SpawnRotation, ActorSpawnParams);
					}
				}
			}
		}
	}

	// on every frame change rotationg for a smooth rotating actor
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

// Get this actors location and create the explosion there
UParticleSystemComponent* AAI_Bot_Enemy4::CreateExplosion(UParticleSystem* Explosion)
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
UParticleSystemComponent* AAI_Bot_Enemy4::CreateSpark(UParticleSystem* Spark)
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
void AAI_Bot_Enemy4::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// On beginoverlap, launch this character in the direction of the vector
void AAI_Bot_Enemy4::OnProjectileEnterFrontSensor(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// If this actor has collision with laser2 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("SpaceShooterCharacter")) {

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());
		}

		// if this actor has a collision. Create a this effect.
		UParticleSystemComponent* Spark = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkSystem, GetActorTransform());
		Spark->SetRelativeScale3D(FVector(2.f));
	}

	// If this enemy character collides with the main character. Then launch this character in this direction.
	FVector launchVelocity = FVector(-2000.0f, 0.0f, 0.0f);

	LaunchCharacter(launchVelocity, false, true);


}

// Generic team ID
FGenericTeamId AAI_Bot_Enemy4::GetGenericTeamId() const
{
	return FGenericTeamId(10);
}

// Attach the eyes to the bone stated in quotation
void AAI_Bot_Enemy4::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation("head");
	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}

// If we are firing set this delay
void AAI_Bot_Enemy4::ShotTimerExpired()
{
	bCanFire = true;
}
