// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShooterCharacter.generated.h"

UCLASS(config=Game)
class ASpaceShooterCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Location mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class USceneComponent* laser_cannon;

	/** Location mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* Rocket_cannon;

	/** Location mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USceneComponent* Left_Light_Socket;

	/** Side view camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
protected:

	// Reference to this Player in World
	UPROPERTY()
	class AAI_Bot_Enemy1* EnemyShield1;

	// Reference to this Player in World
	UPROPERTY()
	class AAI_Bot_Enemy2* EnemyShield2;

	/** Can launched this character */
	UFUNCTION()
		virtual void Dash(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);

	/** Called for side to side input */
	void MoveRight(float Val);
		
	/** Called for up to down input */
	void MoveUpDown(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/** laser offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector laser_cannonOffset;

	/** Rocket offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector Rocket_cannonOffset;

	/** Left light offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector Left_Light_SocketOffset;
public:
	ASpaceShooterCharacter(const FObjectInitializer& ObjectInitializer);

	// Can Save game.
	UFUNCTION()
	void SaveGame();

	// Can Load game.
	UFUNCTION()
	void LoadGame();
	
	// Static Mesh Component for planet Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* SpaceShip;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;
	
	/* How fast we dash will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float DashRate;
	
	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	/* Handler for the Dash timer expiry */
	void DashTimerExpired();

	// Use engine particle system to create an explosion
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* ExplosionSystem;
	class UParticleSystemComponent* CreateExplosion(class UParticleSystem* Explosion);

	// Use engine particle system to create a Spark
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* SparkSystem;
	class UParticleSystemComponent* CreateSpark(class UParticleSystem* Spark);

	// Use engine particle system to create a DashStream
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* DashStreamSystem;
	class UParticleSystemComponent* CreateDashStream(class UParticleSystem* DashStream);

	// Use engine particle system to create a DashStream
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* SmokeSystem;
	class UParticleSystemComponent* CreateSmoke(class UParticleSystem* Smoke);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Sound to play each time we do the Laser1Sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Laser1Sound;

	/** Sound to play each time we do the BeHitSound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* BeHitSound;

	/** Sound to play each time we do the DashSound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* DashSound;

	/** Sound to play each time we do the MissileSound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* MissileSound;

// Set up for Score amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Score;

	// Set up for Score amount value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Score_Treshold;

	// Set up for Shield amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Shield;

	// Set up for Shield amount value
	UPROPERTY(EditAnywhere)
		float Shield_Treshold;

	// Set up for dash amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DashCharge;

	// Set up for dash amount value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DashCharge_Treshold;

	 //Set up for Laser amount 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Laser;

	// Set up for Laser amount value
	UPROPERTY(EditAnywhere)
		float Laser_Treshold;

	// Set up for Rockets amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Rockets;

	// Set up for Rocket amount value
	UPROPERTY(EditAnywhere)
		float Rockets_Treshold;
	
	// Set up for character collision
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	
	// For mouse cursot tracking
	void SetCursorDir();
	// for WASD controll;
	void TurnAtRate(float Rate);

	// Can restart game after my character death.
	void RestartGame();

	// Can Die Bool
	bool bDead;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Left light class
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ALeft_ShipLight> Left_ShipLightclass;
	class ALeft_ShipLight* Left_ShipLight;

private:

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	/** Handle for efficient management of DashTimerExpired timer */
	FTimerHandle TimerHandle_DashTimerExpired;


	/** laser1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ALaser1> ALaser1class;

	// Set up for laser cannon 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* Laser1Fire;

	// Set up for laser cannon 1 animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* Missile1Fire;

	// Set up for Equipped sword animation.
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* RemoveLight;

	
	/** Rocket1 class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AMissile> AMissileclass;
	
	// Toggle light on
	void LightsOn();

	// Toggle light off
	void LightsOff();

	// Can fire laser
	void FirePrimary();
	
	// Can fire Rocket
	void FireSecondary();

	// Can DashBack
	void DashBack();

	// Can DashForward
	void DashForward();

	// Can DashLeft
	void DashLeft();

	// Can DashRight
	void DashRight();
	
	/* Flag to control firing  */
	uint32 bCanFire :1;

	/* Flag to control firing  */
	uint32 bCanDash :1;

	// Implement The Generic Team Interface 
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;
};
