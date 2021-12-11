// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Asteroid_Projectile_3.generated.h"

UCLASS()
class SPACESHOOTER_API AAsteroid_Projectile_3 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAsteroid_Projectile_3();

	// Creates a body for this Pawn
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;

	// Static Mesh Component for planet Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* Asteroid;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	// declare our float variables 	
	UPROPERTY(EditAnywhere, Category = Movement)
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float RollValue;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	// Use engine particle system to create a FireSmoke
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* FireSmokeSystem;
	class UParticleSystemComponent* CreateFireSmoke(class UParticleSystem* FireSmoke);

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
