// Fill out your copyright notice in the Description page of Project Settings.


#include "Gas_Giant_Atmosphere_1.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGas_Giant_Atmosphere_1::AGas_Giant_Atmosphere_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a body for this mesh and is attacted to the root component
	Atmosphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Atmosphere"));
	Atmosphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGas_Giant_Atmosphere_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGas_Giant_Atmosphere_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

