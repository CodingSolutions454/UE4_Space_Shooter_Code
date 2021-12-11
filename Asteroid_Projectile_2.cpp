// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid_Projectile_2.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAsteroid_Projectile_2::AAsteroid_Projectile_2()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a ProjectileMovementComponent to govern this projectile's movement.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

	//Creates a body for this Laser.
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;

	// Creates a body for this mesh and is attacted to the root component
	Asteroid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid"));
	Asteroid->SetupAttachment(RootComponent);

	// Create a collision box for this actor
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	BoxComponent->SetupAttachment(GetRootComponent());

	// Set this rotation
	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;

	ProjectileMovement->InitialSpeed = 600.f;
	ProjectileMovement->MaxSpeed = 600.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	// Destroy after 5 seconds by default
	InitialLifeSpan = 60.0f;
}


// Create this effect during this actors life time
UParticleSystemComponent* AAsteroid_Projectile_2::CreateFireSmoke(UParticleSystem* FireSmoke)
{
	{
		class UParticleSystemComponent* retVal = NULL;

		if (FireSmoke)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, FireSmoke, myPos, myRot, true);
			}
		}

		return retVal;
	}
}

// if we hit something
void AAsteroid_Projectile_2::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

// Called when the game starts or when spawned
void AAsteroid_Projectile_2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAsteroid_Projectile_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// on every frame change rotationg for a smooth rotating actor
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

	// Create this effect in tick
	UParticleSystemComponent* FireSmoke = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireSmokeSystem, GetActorTransform());
	FireSmoke->SetRelativeScale3D(FVector(2.f));
}