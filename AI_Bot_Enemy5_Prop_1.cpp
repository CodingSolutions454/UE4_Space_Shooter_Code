// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Enemy5_Prop_1.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AAI_Bot_Enemy5_Prop_1::AAI_Bot_Enemy5_Prop_1()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates a body for this Laser.
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;
	
	//Construct Static Mesh Component
	UFOShip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOShip"));
	UFOShip->SetupAttachment(RootComponent);
	
	//Construct Static Mesh Component
	UFOLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOLight"));
	UFOLight->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	// Destroy after 5 seconds by default
	InitialLifeSpan = 2000.0f;
}

// Called when the game starts or when spawned
void AAI_Bot_Enemy5_Prop_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_Bot_Enemy5_Prop_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}