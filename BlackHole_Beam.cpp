// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole_Beam.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole_Beam::ABlackHole_Beam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	BlackHoleBeam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portel"));
	BlackHoleBeam->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABlackHole_Beam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole_Beam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

