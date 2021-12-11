// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid_9.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAsteroid_9::AAsteroid_9()
{
	// Creates a body for this mesh and is attacted to the root component
	Asteroid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid"));
	Asteroid->SetupAttachment(RootComponent);

	// Destroy after 0 seconds by default
	InitialLifeSpan = 0.0f;
}

