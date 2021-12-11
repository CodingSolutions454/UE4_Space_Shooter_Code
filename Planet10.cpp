// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet10.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlanet10::APlanet10()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	Saturn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planet"));
	Saturn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlanet10::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlanet10::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

