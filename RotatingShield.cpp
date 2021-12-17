// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingShield.h"
#include "AI_Bot_Enemy4.h"

// Sets default values
ARotatingShield::ARotatingShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PitchValue = 0.f;
	YawValue = 50.f;
	RollValue = 0.f;
}

// Called every frame
void ARotatingShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// on every frame change rotationg for a smooth rotating actor
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

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

void ARotatingShield::BeginPlay()
{
	Super::BeginPlay();

	
}
