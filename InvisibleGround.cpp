// Fill out your copyright notice in the Description page of Project Settings.


#include "InvisibleGround.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInvisibleGround::AInvisibleGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Construct Static Mesh Component
	InvisibleGround = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvisibleGround"));
	InvisibleGround->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInvisibleGround::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInvisibleGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

