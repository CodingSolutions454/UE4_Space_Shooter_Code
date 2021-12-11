// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Native construct to associate our buttons with our OnClicked events.
void UInGameUI::NativeConstruct()
{
	if (NewGameButton) 
	{
		NewGameButton->OnClicked.AddDynamic(this,&UInGameUI::NewGameOnClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this,&UInGameUI::QuitOnClicked);
	}
}

// If a new game is created the open level 1
void UInGameUI::NewGameOnClicked()
{
	RemoveFromParent();
	
	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();
	
	UGameplayStatics::OpenLevel(this, "Level_1");
}

void UInGameUI::LoadGameOnClicked()
{
	RemoveFromParent();

	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();

	UGameplayStatics::OpenLevel(this, "Level_1");
}


void UInGameUI::SettingsGameOnClicked()
{
}

void UInGameUI::CreditsGameOnClicked()
{
}

// Quit game event
void UInGameUI::QuitOnClicked()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

