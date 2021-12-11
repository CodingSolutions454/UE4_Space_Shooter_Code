// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Bot_Enemy5_Movement_1.generated.h"

UCLASS()
class SPACESHOOTER_API AAI_Bot_Enemy5_Movement_1 : public AActor
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
	AAI_Bot_Enemy5_Movement_1(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Reference To Our Player in World
	UPROPERTY()
		class ASpaceShooterCharacter* PlayerInWorld;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Allow a Short Delay
	void DelayLogic(float dTime);

	// Find Target In World
	void FindPlayer();
	void UpdateTarget();

	// Static Mesh Component for TrackingLaser1 Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* Movement_Mesh;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
