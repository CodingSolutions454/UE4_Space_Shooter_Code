// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AI_Bot_Enemy1_Prop_1.generated.h"

UCLASS()
class SPACESHOOTER_API AAI_Bot_Enemy1_Prop_1 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAI_Bot_Enemy1_Prop_1();

	// Creates a body for this Pawn
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;

	// Static Mesh Component for this Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* UFOShip;

	// Static Mesh Component for this Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* UFOLight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
