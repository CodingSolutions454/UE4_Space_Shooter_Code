// Fill out your copyright notice in the Description page of Project Settings.


#include "Space_SkyBox.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASpace_SkyBox::ASpace_SkyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	SkyBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portel"));
	SkyBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpace_SkyBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpace_SkyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

