// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet1.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlanet1::APlanet1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this rotation
	PitchValue = 0.f;
	YawValue = -0.001f;
	RollValue = 0.f;

	//Construct Static Mesh Component
	Earth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Earth"));
	Earth->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlanet1::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlanet1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// on every frame change rotationg for a smooth rotating actor
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

