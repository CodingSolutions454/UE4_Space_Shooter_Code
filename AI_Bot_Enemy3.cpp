// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Enemy3.h"
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
AAI_Bot_Enemy3::AAI_Bot_Enemy3(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		SetGenericTeamId(FGenericTeamId(5));
	}
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);

	// Create a left sensor for this actor
	LeftSensor = CreateDefaultSubobject<USphereComponent>(TEXT("LeftSensor"));
	LeftSensor->SetupAttachment(GetRootComponent());
	LeftSensor->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy3::OnProjectileEnterLeftSensor);

	// Create a right Senser for this actor
	RightSensor = CreateDefaultSubobject<USphereComponent>(TEXT("RightSensor"));
	RightSensor->SetupAttachment(GetRootComponent());
	RightSensor->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy3::OnProjectileEnterRightSensor);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	//Bools set here
	bDead = false;
	Detected = false;

	//Position laser cannon is fired from
	RightLaserTurrentOffset = FVector(100.0f, 0.0f, 10.0f);

	//Position laser cannon is fired from
	LeftLaserTurrentOffset = FVector(100.0f, 0.0f, 10.0f);

	//float values
	EnemyShield3 = 100.0f;
	FireRate = 6.0f;

	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .25f; // 4 times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

void AAI_Bot_Enemy3::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{

	//If this actor has collision with laser1 subtract amount of shield then destroy other colliding actor
	if (OtherActor->ActorHasTag("Laser1")) {

		EnemyShield3 -= 26.0f;

		if (EnemyShield3 > 100.0f)
			EnemyShield3 = 100.0f;

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

		EnemyShield3 -= 200.0f;

		if (EnemyShield3 > 100.0f)
			EnemyShield3 = 100.0f;

		OtherActor->Destroy();

		// try and play the sound if specified
		if (BeHitSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BeHitSound, GetActorLocation());

		}
	}
}

// OnPawn detected, cast to main charcater and fire a prjectile towrads the main character. Play Sound and create animation
void AAI_Bot_Enemy3::SeePawn(APawn* Pawn)
{
	if (PawnSensing->OnSeePawn, true) {

		ASpaceShooterCharacter* Actor = Cast<ASpaceShooterCharacter>(PawnSensing);

		// try and fire a Laser3
		if (ALaser3class != NULL)
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

					// spawn the Laser2 at the laser cannon bone
					World->SpawnActor<ALaser3>(ALaser3class, SpawnLocation, SpawnRotation, ActorSpawnParams);

					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAI_Bot_Enemy3::ShotTimerExpired, FireRate);

					// Plays animation for whne laser is being fired.
					GetMesh()->SetAnimation(Laser3Fire);
					GetMesh()->PlayAnimation(Laser3Fire, false);

					// try and play the sound if specified
					if (Laser3Sound != NULL)
					{
						UGameplayStatics::PlaySoundAtLocation(this, Laser3Sound, GetActorLocation());
					}
				}
			}
		}
	}

	if (PawnSensing->OnSeePawn, true) {

		ASpaceShooterCharacter* Actor = Cast<ASpaceShooterCharacter>(PawnSensing);

		// try and fire a Laser4
		if (ALaser4class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				{
					const FRotator SpawnRotation = GetActorRotation();

					const FVector SpawnLocation = ((LeftLaserTurrent != nullptr) ? LeftLaserTurrent->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(LeftLaserTurrentOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the Laser2 at the laser cannon bone
					World->SpawnActor<ALaser4>(ALaser4class, SpawnLocation, SpawnRotation, ActorSpawnParams);

					World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAI_Bot_Enemy3::ShotTimerExpired, FireRate);

					// Plays animation for whne laser is being fired.
					GetMesh()->SetAnimation(Laser4Fire);
					GetMesh()->PlayAnimation(Laser4Fire, false);

					// try and play the sound if specified
					if (Laser4Sound != NULL)
					{
						UGameplayStatics::PlaySoundAtLocation(this, Laser4Sound, GetActorLocation());
					}

					bCanFire = false;
				}
			}
		}
	}
}

void AAI_Bot_Enemy3::Dodge(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	// Lanch Character logic is used in the left and right sensor. If we hit something
}

//Called when the game starts or when spawned
void AAI_Bot_Enemy3::BeginPlay()
{
	Super::BeginPlay();

	// Get capsule component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy3::OnBeginOverLap);

	PawnSensing->OnSeePawn.AddDynamic(this, &AAI_Bot_Enemy3::SeePawn);
}

// Called every frame
void AAI_Bot_Enemy3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// If EnemyShield is less than 0. Simulate physics, play animation, play sound then destroy the capsule to avoid collisions
	if (EnemyShield3 < 1) {

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
UParticleSystemComponent* AAI_Bot_Enemy3::CreateExplosion(UParticleSystem* Explosion)
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
UParticleSystemComponent* AAI_Bot_Enemy3::CreateSpark(UParticleSystem* Spark)
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
void AAI_Bot_Enemy3::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// On beginoverlap, launch this character in the direction of the vector
void AAI_Bot_Enemy3::OnProjectileEnterLeftSensor(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector launchVelocity = FVector(1000.0f, 0.0f, 0.0f);

	LaunchCharacter(launchVelocity, false, true);
}

// On beginoverlap, launch this character in the direction of the vector
void AAI_Bot_Enemy3::OnProjectileEnterRightSensor(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector launchVelocity = FVector(-1000.0f, 0.0f, 0.0f);

	LaunchCharacter(launchVelocity, false, true);
}

// Generic team ID
FGenericTeamId AAI_Bot_Enemy3::GetGenericTeamId() const
{
	return FGenericTeamId(10);
}

// Attach the eyes to the bone stated in quotation
void AAI_Bot_Enemy3::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation("head");
	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}

// If we are firing set this delay
void AAI_Bot_Enemy3::ShotTimerExpired()
{
	bCanFire = true;
}
