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
#include "Blueprint/UserWidget.h"
#include "AIController.h"
#include "SpaceShooterCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "AI_Bot_Controller.generated.h"

/**
 *
 */
UCLASS()
class SPACESHOOTER_API AAI_Bot_Controller : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Bot_Controller();

	// Override this function 
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Possess this pawn as a controller
	virtual void OnPossess(APawn* OnPawn) override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Get this pawns rotation
	virtual FRotator GetControlRotation() const override;

	// Can detect pawn
	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawn);

	// AI sight range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 1000.0f;

	//AI sight age
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 10.0f;

	// AI lose sight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AILosesRadius = AISightAge + 50.0f;

	// AI field of few
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfview = 100.0f;

	// AI sight config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	// AI player detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

	// AI set distance to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f;
};