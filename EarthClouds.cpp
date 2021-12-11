// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthClouds.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEarthClouds::AEarthClouds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this rotation
	PitchValue = 0.f;
	YawValue = 0.0030f;
	RollValue = 0.f;

	//Construct Static Mesh Component
	Clouds = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planet"));
	Clouds->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEarthClouds::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEarthClouds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// on every frame change rotationg for a smooth rotating actor
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

