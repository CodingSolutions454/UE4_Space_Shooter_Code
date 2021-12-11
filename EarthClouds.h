// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EarthClouds.generated.h"

UCLASS()
class SPACESHOOTER_API AEarthClouds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEarthClouds();

	// declare our float variables 	
	UPROPERTY(EditAnywhere, Category = Movement)
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float RollValue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* Clouds;
};