// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid_Spawn_1.h"
#include "Asteroid_Projectile_1.h"
#include "SpaceShooterCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values
AAsteroid_Spawn_1::AAsteroid_Spawn_1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Position laser cannon is fired from
	Asteroid_Spawn_BoneCannonOffset = FVector(100.0f, 0.0f, 10.0f);

	DoOnce = true;

	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .25f; // 4 times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

// OnPawn detected, cast to main charcater and fire a projectile towrads the main character. Play Sound and create animation
void AAsteroid_Spawn_1::SeePawn(APawn* Pawn)
{
	if (DoOnce == true) 
	{
		if (PawnSensing->OnSeePawn, true) {

			ASpaceShooterCharacter* Actor = Cast<ASpaceShooterCharacter>(PawnSensing);

			// try and fire a Laser2
			if (AAsteroid_Projectile_1class != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					{
						const FRotator SpawnRotation = GetActorRotation();

						const FVector SpawnLocation = ((Asteroid_Spawn_Bone != nullptr) ? Asteroid_Spawn_Bone->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(Asteroid_Spawn_BoneCannonOffset);

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the Laser2 at the laser cannon bone
						World->SpawnActor<AAsteroid_Projectile_1>(AAsteroid_Projectile_1class, SpawnLocation, SpawnRotation, ActorSpawnParams);

						DoOnce = false;
					
					}
				}
			}
		}
	}
}

	// Called when the game starts or when spawned
void AAsteroid_Spawn_1::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &AAsteroid_Spawn_1::SeePawn);

}

// Called every frame
void AAsteroid_Spawn_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
} 

