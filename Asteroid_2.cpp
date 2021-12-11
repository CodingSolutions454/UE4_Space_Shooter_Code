// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid_2.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAsteroid_2::AAsteroid_2()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a body for this mesh and is attacted to the root component
	Asteroid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid"));
	Asteroid->SetupAttachment(RootComponent);

	// Destroy after 0 seconds by default
	InitialLifeSpan = 0.0f;
}