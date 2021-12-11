// Fill out your copyright notice in the Description page of Project Settings.

//#include "FlashLight/Public/Left_ShipLight.h"
#include "Left_ShipLight.h"
#include "Components/SpotLightComponent.h"

// Sets default values
ALeft_ShipLight::ALeft_ShipLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Construct SpotLight Component
	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(RootComponent);

	//Construct Static Mesh Component
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	LightMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALeft_ShipLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeft_ShipLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

