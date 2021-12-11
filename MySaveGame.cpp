// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

UMySaveGame::UMySaveGame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Save game any loaction
	PlayerLocation = FVector(0.f, 0.f, 0.f);
}