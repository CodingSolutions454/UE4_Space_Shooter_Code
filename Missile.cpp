// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "AI_Bot_Enemy1.h"
#include "AI_Bot_Enemy2.h"
#include "SpaceShooter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMissile::AMissile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Construct Static Mesh Component
	MissileMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("MissileMesh"));
	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<StaticMesh>"));
	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh"));// We do not need this as we will create a BP class that derives from this C++ class
	//MissileMesh->SetStaticMesh(MeshObj.Object);
	MissileMesh->SetupAttachment(RootComponent);

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

	// Destroy after 10 seconds by default
	InitialLifeSpan = 10.0f;

	// Set Default Values for Variables
	hasTargetPosition = false;
	hasNoTarget = false;
	target = NULL;
	delayTimer = 0.5f;
	hasFinishedDelay = false;
	lifetimeCountdown = 15.f;
	canBeDestroyed = false;
	PlayerInWorld = NULL;
}

UParticleSystemComponent* AMissile::CreateSmokeStream(UParticleSystem* SmokeStream)
{
	{
		class UParticleSystemComponent* retVal = NULL;

		if (SmokeStream)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, SmokeStream, myPos, myRot, true);
			}
		}

		return retVal;
	}

}

#pragma region Setup Target Logic
// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	FindPlayer();

	// Before we find our target, launch the Missile upwards
	if (!hasTargetPosition)
	{
		ProjectileMovement->Velocity = GetActorUpVector() * 100.f;

		// BeginPlay with collision disabled so that the missile doesn't interact with other actors
		this->SetActorEnableCollision(false);
	}
}

// Find thisthe enemy in world
void AMissile::FindPlayer()
{
	class UWorld* const world = GetWorld();

	if (world)
	{
		for (TActorIterator<AAI_Bot_Enemy1> ObstacleItr(world); ObstacleItr; ++ObstacleItr)
		{
			FName PlayerTagName = FName(TEXT("AI_Bot_Enemy1"));
			class AAI_Bot_Enemy1* FoundPlayer = *ObstacleItr;

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
void AMissile::UpdateTarget()
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
				FRotator rotVal = MissileMesh->GetComponentRotation();
				rotVal.Roll = 0.f;
				rotVal.Pitch = -90.f;
				rotVal.Yaw = 0.f;
				MissileMesh->SetRelativeRotation(rotVal);
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
void AMissile::DelayLogic(float dTime)
{
	if (!hasFinishedDelay)
	{
		delayTimer += 1* dTime;

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
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Create this effect when spawned in the world
	UParticleSystemComponent* SmokeStream = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeStreamSystem, GetActorTransform());
	SmokeStream->SetRelativeScale3D(FVector(0.7f));

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

		// Destroy the missile actor after a 10 Second countdown
		lifetimeCountdown -= 1 * DeltaTime;

		if (lifetimeCountdown < 10.f)
		{
			if (!canBeDestroyed)
			{
				canBeDestroyed = true;
				target = nullptr;

			}
		}
	}
}
