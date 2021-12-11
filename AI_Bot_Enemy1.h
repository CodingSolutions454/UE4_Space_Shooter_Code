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
#include "AI_Bot_Enemy1.generated.h"

class UPawnSensingComponent;

UCLASS()
class SPACESHOOTER_API AAI_Bot_Enemy1 : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
	/** Sphere collision component */
	UPROPERTY(EditAnywhere, Category = Projectile)
	class USphereComponent* CollisionComp;

	/** Location mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* EnemyLaserCannon;

public:
	// Sets default values for this character's properties
	AAI_Bot_Enemy1(const FObjectInitializer& ObjectInitializer);

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
		FVector EnemyLaserCannonOffset;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static Mesh Component for this Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* UFOShip;
	
	// Static Mesh Component for this Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* UFOLight;

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

	/** Sound to play each time we do the Laser1Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Laser2Sound;

	/** Sound to play each time we do the Laser1Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ExplosionSound;

	// Set up for EnemyShield amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnemyShield1;

	// Set up for EnemyShield amount value
	UPROPERTY(EditAnywhere)
		float Enemy1_Treshold;

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	// Get this actors location
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	/** laser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ALaser2> ALaser2class;

	/** laser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AScore_50> AScore_50class;

	// Set up for laser cannon 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* Laser2Fire;
	
	// Implement The Generic Team Interface 
	
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;
};

