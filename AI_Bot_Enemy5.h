// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "AI_Bot_Enemy5.generated.h"

class UPawnSensingComponent;

UCLASS()
class SPACESHOOTER_API AAI_Bot_Enemy5 : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

		/** Location mesh where projectiles should spawn. */
		UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* RightLaserTurrent;

	/** Location mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* LeftLaserTurrent;

public:
	// Sets default values for this character's properties
	AAI_Bot_Enemy5(const FObjectInitializer& ObjectInitializer);

	// Static Mesh Component for this Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* UFOShip;

	// Static Mesh Component for this Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* UFOLight;

	/** Sound to play each time we do the BeHitSound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BeHitSound;

	// Set up for character collision
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

protected:

	// Reference To Our Player in World
	UPROPERTY()
		class ASpaceShooterCharacter* Score;

	// Create Pawnsensing component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UPawnSensingComponent* PawnSensing;

	// For OnPawn Detected
	UFUNCTION()
		void SeePawn(APawn* Pawn);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** EnemyLaserCannon offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector RightLaserTurrentOffset;

	/** EnemyLaserCannon offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector LeftLaserTurrentOffset;

	/** Arrow offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector laser_cannonOffset;

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

	// Use engine particle system to create a Spark
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* SparkSystem;
	class UParticleSystemComponent* CreateSpark(class UParticleSystem* Spark);

	/** Sound to play each time we do the Laser3Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Laser5Sound;

	/** Sound to play each time we do the Laser1Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ExplosionSound;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	// Set up for EnemyShield amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnemyShield5;

	// Set up for EnemyShield amount value
	UPROPERTY(EditAnywhere)
		float Enemy5_Treshold;

	// Get this actors location
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	/** TrackingLaser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ALaser5> ALaser5class;

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
