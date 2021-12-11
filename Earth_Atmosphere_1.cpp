// Fill out your copyright notice in the Description page of Project Settings.


#include "Earth_Atmosphere_1.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEarth_Atmosphere_1::AEarth_Atmosphere_1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	Atmosphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Earth"));
	Atmosphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEarth_Atmosphere_1::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEarth_Atmosphere_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}