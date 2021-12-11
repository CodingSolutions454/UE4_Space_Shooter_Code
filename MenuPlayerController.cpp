// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"
#include "MenuHUD.h"

AMenuPlayerController::AMenuPlayerController() 
{

}

// Can open this menu
void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{

		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AMenuPlayerController::OpenMenu);

	}
}

// Get this hud and show menu
void AMenuPlayerController::OpenMenu()
{
	if (AMenuHUD* MenuHUD = Cast<AMenuHUD>(GetHUD()))
    {

		MenuHUD->ShowMenu();
    }

}
