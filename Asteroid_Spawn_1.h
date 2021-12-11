// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AISightTargetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Asteroid_Spawn_1.generated.h"

class UPawnSensingComponent;

UCLASS()
class SPACESHOOTER_API AAsteroid_Spawn_1 : public ACharacter
{
	GENERATED_BODY()

		/** Location mesh where projectiles should spawn. */
		UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* Asteroid_Spawn_Bone;

public:
	// Sets default values for this character's properties
	AAsteroid_Spawn_1();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** EnemyLaserCannon offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector  Asteroid_Spawn_BoneCannonOffset;


	// Create Pawnsensing component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UPawnSensingComponent* PawnSensing;

	// For OnPawn Detected
	UFUNCTION()
		void SeePawn(APawn* Pawn);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool DoOnce;

private:

	/** Asteroid_Projectile_1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AAsteroid_Projectile_1> AAsteroid_Projectile_1class;
};
