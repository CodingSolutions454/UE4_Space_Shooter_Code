// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole_Planet_Atmosphere.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole_Planet_Atmosphere::ABlackHole_Planet_Atmosphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	Atmosphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Earth"));
	Atmosphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlackHole_Planet_Atmosphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole_Planet_Atmosphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

