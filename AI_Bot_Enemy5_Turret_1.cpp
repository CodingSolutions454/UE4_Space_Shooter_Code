// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Enemy5_Turret_1.h"
#include "SpaceShooterCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAI_Bot_Enemy5_Turret_1::AAI_Bot_Enemy5_Turret_1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Position laser cannon is fired from
	TurretOffset = FVector(100.0f, 0.0f, 10.0f);

	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .25f; // 4 times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

// Called when the game starts or when spawned
void AAI_Bot_Enemy5_Turret_1::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAI_Bot_Enemy5_Turret_1::SeePawn(APawn* Pawn)
{
	if (PawnSensing->OnSeePawn, true) {

		ASpaceShooterCharacter* Actor = Cast<ASpaceShooterCharacter>(PawnSensing);

		// try and fire a Laser3
		if (ALaser3class != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				{
					const FRotator SpawnRotation = GetActorRotation();

					const FVector SpawnLocation = ((Turret != nullptr) ? Turret->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(TurretOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the Laser2 at the laser cannon bone
					World->SpawnActor<ALaser3>(ALaser3class, SpawnLocation, SpawnRotation, ActorSpawnParams);

					// try and play the sound if specified
					if (Laser3Sound != NULL)
					{
						UGameplayStatics::PlaySoundAtLocation(this, Laser3Sound, GetActorLocation());
					}
				}
			}
		}
	}

}

// Called every frame
void AAI_Bot_Enemy5_Turret_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}