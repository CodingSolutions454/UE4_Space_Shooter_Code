// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingShield.generated.h"

UCLASS()
class SPACESHOOTER_API ARotatingShield : public AActor
{
	GENERATED_BODY()
	
protected:

	
public:	
	// Sets default values for this actor's properties
	ARotatingShield();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// declare our float variables 	
	UPROPERTY(EditAnywhere, Category = Movement)
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float RollValue;
};
