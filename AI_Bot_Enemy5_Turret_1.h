// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "AI_Bot_Enemy5_Turret_1.generated.h"

class UPawnSensingComponent;

UCLASS()
class SPACESHOOTER_API AAI_Bot_Enemy5_Turret_1 : public ACharacter
{
	GENERATED_BODY()

		/** Location mesh where projectiles should spawn. */
		UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* Turret;

public:
	// Sets default values for this character's properties
	AAI_Bot_Enemy5_Turret_1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** EnemyLaserCannon offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector TurretOffset;

	// Create Pawnsensing component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UPawnSensingComponent* PawnSensing;

	// For OnPawn Detected
	UFUNCTION()
		void SeePawn(APawn* Pawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Sound to play each time we do the Laser3Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Laser3Sound;
private:
	/** TrackingLaser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ALaser3> ALaser3class;
};
