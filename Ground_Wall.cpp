// Fill out your copyright notice in the Description page of Project Settings.


#include "Ground_Wall.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGround_Wall::AGround_Wall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Construct Static Mesh Component
	Ground_Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground_Wall"));
	Ground_Wall->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGround_Wall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGround_Wall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

