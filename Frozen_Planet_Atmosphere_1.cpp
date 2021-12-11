// Fill out your copyright notice in the Description page of Project Settings.


#include "Frozen_Planet_Atmosphere_1.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AFrozen_Planet_Atmosphere_1::AFrozen_Planet_Atmosphere_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	Atmosphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planet4"));
	Atmosphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFrozen_Planet_Atmosphere_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFrozen_Planet_Atmosphere_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

