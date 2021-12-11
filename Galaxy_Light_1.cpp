// Fill out your copyright notice in the Description page of Project Settings.


#include "Galaxy_Light_1.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGalaxy_Light_1::AGalaxy_Light_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light"));
	LightMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGalaxy_Light_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGalaxy_Light_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

