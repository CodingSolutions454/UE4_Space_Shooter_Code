// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser_Light.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALaser_Light::ALaser_Light()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	LaserLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserLight"));
	LaserLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALaser_Light::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaser_Light::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

