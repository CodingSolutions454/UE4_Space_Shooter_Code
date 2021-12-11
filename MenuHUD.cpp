// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"
#include "SMainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/PlayerController.h"

// At the start of the game. Show the main menu
void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowMenu();
}

// At the start of the game show the menu and add it to viewport
 void AMenuHUD::ShowMenu()
 {
	if (GEngine && GEngine->GameViewport)
	{
		MenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));

		if (PlayerOwner)
	    {
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
 }

 //When the game is launched set the main menu to be hidden and create the other widgets
void AMenuHUD::HideMenu()
{

	if (GEngine && GEngine->GameViewport && MenuWidget.IsValid() && MenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeGameAndUI());
		}
	}

	// Create these widgets when the MenuHUD is hidden and only when the MenuHUD is hidden.
	if (Player_Shield_Widget_Class != nullptr) {

		Player_Shield_Widget = CreateWidget(GetWorld(), Player_Shield_Widget_Class);
		Player_Shield_Widget->AddToViewport();
	}
	if (Player_Laser_Cannon_Widget_Class != nullptr) {

		Player_Laser_Cannon_Widget = CreateWidget(GetWorld(), Player_Laser_Cannon_Widget_Class);
		Player_Laser_Cannon_Widget->AddToViewport();
	}
	if (Player_Tracking_Rocket_Widget_Class != nullptr) {

		Player_Tracking_Rocket_Widget = CreateWidget(GetWorld(), Player_Tracking_Rocket_Widget_Class);
		Player_Tracking_Rocket_Widget->AddToViewport();
	}
	if (Player_Score_Widget_Class != nullptr) {

		Player_Score_Widget = CreateWidget(GetWorld(), Player_Score_Widget_Class);
		Player_Score_Widget->AddToViewport();
	}
	if (Player_Dash_Widget_Class != nullptr) {

		Player_Dash_Widget = CreateWidget(GetWorld(), Player_Dash_Widget_Class);
		Player_Dash_Widget->AddToViewport();
	}
}
