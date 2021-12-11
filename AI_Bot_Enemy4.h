// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/inputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AISightTargetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "AIController.h"
#include "AI_Bot_Enemy4.generated.h"

class UPawnSensingComponent;

UCLASS()
class SPACESHOOTER_API AAI_Bot_Enemy4 : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Bot_Enemy4(const FObjectInitializer& ObjectInitializer);

	/** Location mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* laser_cannon;

	/** Sound to play each time we do the BeHitSound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BeHitSound;

	// Set up for character collision
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// Right sensor collision
	UPROPERTY(EditAnywhere)
		class USphereComponent* FrontSensor;

protected:

	/** Arrow offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector laser_cannonOffset;

	// Reference To Our Player in World
	UPROPERTY()
		class ASpaceShooterCharacter* Score;

	// Create Pawnsensing component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UPawnSensingComponent* PawnSensing;

	// For OnPawn Detected
	UFUNCTION()
		void SeePawn(APawn* Pawn);

	/** Can launched this character */
	UFUNCTION()
		virtual void Dodge(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Can Die Bool
	bool bDead;

	// If this player is detected
	bool Detected;

	// Use engine particle system to create an explosion
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* ExplosionSystem;
	class UParticleSystemComponent* CreateExplosion(class UParticleSystem* Explosion);

	// Use engine particle system to create a spark
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* SparkSystem;
	class UParticleSystemComponent* CreateSpark(class UParticleSystem* Spark);

	/** Sound to play each time we do the Laser3Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Laser3Sound;

	/** Sound to play each time we do the Laser4Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Laser4Sound;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	// Set up for EnemyShield amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnemyShield4;

	// Set up for EnemyShield amount value
	UPROPERTY(EditAnywhere)
		float Enemy4_Treshold;

	// declare our float variables 	
	UPROPERTY(EditAnywhere, Category = Movement)
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float RollValue;

	// Get this actors location
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Set up for left sensor collisions
	UFUNCTION()
		void OnProjectileEnterFrontSensor(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	/** laser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AScore_50> AScore_50class;

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	// Implement The Generic Team Interface 
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;
};


