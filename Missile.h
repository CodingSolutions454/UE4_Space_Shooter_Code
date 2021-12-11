// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

UCLASS()
class SPACESHOOTER_API AMissile : public AActor
{
	GENERATED_BODY()
	
private:

	// Target Variables
	bool hasTargetPosition;
	bool hasNoTarget;
	class APawn* target;

	// Delay Variables
	float delayTimer;
	bool hasFinishedDelay;

	// Actor Lifetime Variables
	float lifetimeCountdown;
	bool canBeDestroyed;

public:

	// Sets default values for this actor's properties
	AMissile(const FObjectInitializer& ObjectInitializer);

	// Use engine particle system to create a smoke stream
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* SmokeStreamSystem;
	class UParticleSystemComponent* CreateSmokeStream(class UParticleSystem* SmokeStream);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Reference To Our Player in World
    UPROPERTY()
		class AAI_Bot_Enemy1* PlayerInWorld;

	// Reference To Our Player in World
	UPROPERTY()
		class AAI_Bot_Enemy2* PlayerWorld;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Allow a Short Delay
	void DelayLogic(float dTime);

	// Find Target In World
	void FindPlayer();
	void UpdateTarget();

	// Static Mesh Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* MissileMesh;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
