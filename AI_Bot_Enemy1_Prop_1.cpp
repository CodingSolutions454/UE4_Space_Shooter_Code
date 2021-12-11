// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Enemy1_Prop_1.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAI_Bot_Enemy1_Prop_1::AAI_Bot_Enemy1_Prop_1()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates a body for this Laser.
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;

	//Construct Static Mesh Component
	UFOShip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOShip"));
	UFOShip->SetupAttachment(RootComponent);

	//Construct Static Mesh Component
	UFOLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOLight"));
	UFOLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAI_Bot_Enemy1_Prop_1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_Bot_Enemy1_Prop_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

