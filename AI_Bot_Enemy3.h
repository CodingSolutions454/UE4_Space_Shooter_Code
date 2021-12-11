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
#include "AI_Bot_Enemy3.generated.h"

class UPawnSensingComponent;

UCLASS()
class SPACESHOOTER_API AAI_Bot_Enemy3 : public ACharacter, public IGenericTeamAgentInterface
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
	AAI_Bot_Enemy3(const FObjectInitializer& ObjectInitializer);

	/** Sound to play each time we do the BeHitSound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BeHitSound;

	// Set up for character collision
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);
	
	// Left sensor collision
	UPROPERTY(EditAnywhere)
		class USphereComponent* LeftSensor;
	
	// Right sensor collision
	UPROPERTY(EditAnywhere)
		class USphereComponent* RightSensor;

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

	/** Can launched this character */
	UFUNCTION()
		virtual void Dodge(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** EnemyLaserCannon offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector RightLaserTurrentOffset;

	/** EnemyLaserCannon offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector LeftLaserTurrentOffset;
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
		class USoundBase* Laser3Sound;

	/** Sound to play each time we do the Laser4Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Laser4Sound;

	/** Sound to play each time we do the Laser1Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ExplosionSound;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	// Set up for EnemyShield amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnemyShield3;

	// Set up for EnemyShield amount value
	UPROPERTY(EditAnywhere)
		float Enemy3_Treshold;

	// Get this actors location
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Set up for left sensor collisions
	UFUNCTION()
		void OnProjectileEnterLeftSensor(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Set up for right sensor collisions
	UFUNCTION()
		void OnProjectileEnterRightSensor(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	/** TrackingLaser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ALaser3> ALaser3class;

	/** TrackingLaser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ALaser4> ALaser4class;

	/** laser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AScore_50> AScore_50class;

	// Set up for laser cannon 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* Laser3Fire;

	// Set up for laser cannon 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* Laser4Fire;

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	// Implement The Generic Team Interface 
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;
};

