// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal_1.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
APortal_1::APortal_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Construct Static Mesh Component
	Portel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portel"));
	Portel->SetupAttachment(RootComponent);

	// Create a collision box for this actor
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	BoxComponent->SetupAttachment(GetRootComponent());

	//Rotate this actor in this position
	PitchValue = 0.f;
	YawValue = 3.f;
	RollValue = 0.f;
}

// Called when the game starts or when spawned
void APortal_1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APortal_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// on every frame change rotationg for a smooth rotating actor
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

	// if this actor has a collision. Create a this effect.
	UParticleSystemComponent* PortalEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalEffectSystem, GetActorTransform());
	Portel->SetRelativeScale3D(FVector(7.f));
	
	// You will want to set rotation with Quanternions and not Rotation values
	// The below method causes a bug for the pitch value. The pitch value stops updating at 90 degrees due to Gimbal Lock

	// FRotator NewRotation = GetActorRotation();

	// NewRotation.Pitch += PitchValue;
	// NewRotation.Yaw += YawValue;
	// NewRotation.Roll += RollValue;

	// OR add values using the FRotator Add function
	// FRotator NewRotation = GetActorRotation().Add(PitchValue, YawValue, RollValue);

	// SetActorRotation(NewRotation);

}

// Get this actors location and create the portal effect 
UParticleSystemComponent* APortal_1::CreatePortalEffect(UParticleSystem* PortalEffect)
{
	{
		class UParticleSystemComponent* retVal = NULL;

		if (PortalEffect)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, PortalEffect, myPos, myRot, true);
			}
		}

		return retVal;
	}
}





