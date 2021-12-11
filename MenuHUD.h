// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	// Share these classes
	TSharedPtr<class SMainMenuWidget> MenuWidget;
	TSharedPtr<class SWidget> MenuWidgetContainer;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	//What happens if the whne the menu is shown
	void ShowMenu();
	
	//What happens when the menu is hidden
	void HideMenu();
	
	// Create these widgets when the menu is hidden in the CPP file.
	///////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Shield_Widget_Class;
	UUserWidget* Player_Shield_Widget;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Laser_Cannon_Widget_Class;
	UUserWidget* Player_Laser_Cannon_Widget;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Tracking_Rocket_Widget_Class;
	UUserWidget* Player_Tracking_Rocket_Widget;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Score_Widget_Class;
	UUserWidget* Player_Score_Widget;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Dash_Widget_Class;
	UUserWidget* Player_Dash_Widget;
	////////////////////////////////////////////////////////////////////////////////////////
};
