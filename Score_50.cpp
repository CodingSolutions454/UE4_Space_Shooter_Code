// Fill out your copyright notice in the Description page of Project Settings.


#include "Score_50.h"
#include "SpaceShooterCharacter.h"
#include "SpaceShooter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AScore_50::AScore_50(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	ScoreMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ScoreMesh"));
	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<StaticMesh>"));
	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh"));// We do not need this as we will create a BP class that derives from this C++ class
	//ScoreMesh->SetStaticMesh(MeshObj.Object);
	ScoreMesh->SetupAttachment(RootComponent);

	// Create a collision box for this actor
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	BoxComponent->SetupAttachment(GetRootComponent());

	// Construct Projectile Movement Component
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));

	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingAccelerationMagnitude = 0.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->Velocity = FVector(0, 0, 0);

	// Set Default Values for Variables
	hasTargetPosition = false;
	hasNoTarget = false;
	target = NULL;
	delayTimer = 0.f;
	hasFinishedDelay = false;
	lifetimeCountdown = 15.f;
	canBeDestroyed = false;
	PlayerInWorld = NULL;
}

#pragma region Setup Target Logic
// Called when the game starts or when spawned
void AScore_50::BeginPlay()
{
	Super::BeginPlay();
	FindPlayer();

	// Before we find our target, launch the Score upwards
	if (!hasTargetPosition)
	{
		ProjectileMovement->Velocity = GetActorUpVector() * 1.f;

		// BeginPlay with collision disabled so that the missile doesn't interact with other actors
		this->SetActorEnableCollision(false);
	}
}

// Find this player in world
void AScore_50::FindPlayer()
{
	class UWorld* const world = GetWorld();

	if (world)
	{
		for (TActorIterator<ASpaceShooterCharacter> ObstacleItr(world); ObstacleItr; ++ObstacleItr)
		{
			FName PlayerTagName = FName(TEXT("SpaceShooterCharacter"));
			class ASpaceShooterCharacter* FoundPlayer = *ObstacleItr;

			if (FoundPlayer != nullptr)
			{
				if (FoundPlayer->ActorHasTag(PlayerTagName))
				{
					if (PlayerInWorld != FoundPlayer)
					{
						PlayerInWorld = FoundPlayer;
					}
				}
			}
		}
	}
}

// Once our delay has finished, we search for possible targets
void AScore_50::UpdateTarget()
{
	if (!hasTargetPosition)
	{
		if (PlayerInWorld != NULL)
		{
			if (PlayerInWorld->IsValidLowLevel())
			{
				target = PlayerInWorld;
				hasTargetPosition = true;
				hasNoTarget = false;

				// Keep Our Mesh Rotation Offset
				FRotator rotVal = ScoreMesh->GetComponentRotation();
				rotVal.Roll = 0.f;
				rotVal.Pitch = -90.f;
				rotVal.Yaw = 0.f;
				ScoreMesh->SetRelativeRotation(rotVal);
			}
			else
			{
				target = nullptr;
				hasTargetPosition = true;
				hasNoTarget = true;
			}
		}
		else
		{
			target = nullptr;
			hasTargetPosition = true;
			hasNoTarget = true;
		}
	}
}

// Allow 1 second to pass before finding our target
void AScore_50::DelayLogic(float dTime)
{
	if (!hasFinishedDelay)
	{
		delayTimer += 1 * dTime;

		if (delayTimer > 1.f)
		{
			UpdateTarget();
			this->SetActorEnableCollision(true);
			hasFinishedDelay = true;
		}
	}
}
#pragma endregion

// Called every frame
void AScore_50::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!hasFinishedDelay)
	{
		DelayLogic(DeltaTime);
	}
	else
	{
		// If a target is found, move the missile actor toward target
		if (hasTargetPosition)
		{
			if (target != NULL)
			{
				if (target->IsValidLowLevel())
				{
					FVector wantedDir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					wantedDir += target->GetVelocity() * wantedDir.Size() / 10000.f;
					ProjectileMovement->Velocity += wantedDir * 10000.f * DeltaTime;
					ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * 10000.f;
				}
				else
				{
					if (!this->IsPendingKill())
						if (this->IsValidLowLevel())
							K2_DestroyActor();
				}
			}
			else
			{
				// If a target is NOT found, continue to move the missile actor upwards
				if (hasNoTarget)
				{
					ProjectileMovement->Velocity = GetActorUpVector() * 200.f;
					hasNoTarget = false;
				}
			}
		}

		// Destroy the Score actor after a 15 Second countdown
		lifetimeCountdown -= 1 * DeltaTime;

		if (lifetimeCountdown < 0.f)
		{
			if (!canBeDestroyed)
			{
				canBeDestroyed = true;
				target = nullptr;

			}
		}

	}
}
