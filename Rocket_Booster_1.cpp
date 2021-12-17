// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket_Booster_1.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ARocket_Booster_1::ARocket_Booster_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Construct Static Mesh Component
	RocketBooster = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketBooster"));
	RocketBooster->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARocket_Booster_1::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARocket_Booster_1::Tick(float DeltaTime)
{

	// Create a this effect.
	UParticleSystemComponent* Booster = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BoosterSystem, GetActorTransform());
	//Spark->SetRelativeScale3D(FVector(2.f));

}

UParticleSystemComponent* ARocket_Booster_1::CreateBooster(UParticleSystem* Booster)
{
	{
		class UParticleSystemComponent* retVal = NULL;

		if (Booster)
		{
			class UWorld* const world = GetWorld();

			if (world)
			{
				FVector myPos = GetActorLocation();
				FRotator myRot = GetActorRotation();

				retVal = UGameplayStatics::SpawnEmitterAtLocation(world, Booster, myPos, myRot, true);
			}
		}

		return retVal;
	}
}




