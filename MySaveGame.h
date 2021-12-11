// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UMySaveGame(const FObjectInitializer& ObjectInitializer);

	// Get players location when saving
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PlayerLocation;
};
