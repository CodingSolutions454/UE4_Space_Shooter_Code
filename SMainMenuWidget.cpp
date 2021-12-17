// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainMenuWidget.h"
#include "MenuHUD.h"
#include "MySaveGame.h"
#include "SpaceShooterCharacter.h"
#include "SpaceShooterGameMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFrameWork/PlayerController.h"

#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments& InArgs) 
{
	// Set Support focus to true
	bCanSupportFocus = true;
	
	//Solve argument for owning hud
	OwningHUD = InArgs._OwningHUD;
	
	// Set button size
	const FMargin ContentPadding = FMargin(800.0f, 400.f);
	const FMargin ButtonPadding = FMargin(10.0f);
	const FMargin ButtonSize = FMargin(10.0f);
	
	// Set text for these buttons
	const FText TitleText = LOCTEXT("GameTitle", "Astro Planes");
	const FText PlayText = LOCTEXT("PlayGame", "Play");
	const FText LoadText = LOCTEXT("LoadGame", "Load"); 
	const FText QuitText = LOCTEXT("QuitGame", "Quit Game");

	// Set font style
	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedStyle");

	// Set test style size
	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 30.f;
	
	// Set childslot
	ChildSlot
	 [
	     SNew(SOverlay)
	     + SOverlay::Slot()
	     .HAlign(HAlign_Fill)
	     .VAlign(VAlign_Fill)
         
		 [
	     
		 SNew(SImage)
	      
		  //Set color to black for image
		 .ColorAndOpacity(FColor::Black)
	     ]
	     
	      + SOverlay::Slot()
		  .HAlign(HAlign_Fill)
		  .VAlign(VAlign_Fill)
          .Padding(ContentPadding)
         [
	            
		  SNew(SVerticalBox)

		  //Title Text
		  + SVerticalBox::Slot() 
		 [
		  SNew(STextBlock)
		  .Font(TitleTextStyle)
		  .Text(TitleText)
		  .Justification(ETextJustify::Center)
			
	     ]
			
		   //Play Button 
		   + SVerticalBox::Slot()
		  .Padding(ButtonPadding)
		 
		 [
		
		   SNew(SButton)
		  .OnClicked(this, &SMainMenuWidget::OnPlayClicked)
				
		 [
		   SNew(STextBlock)
		  .Font(TitleTextStyle)
		  .Text(PlayText)
		  .Justification(ETextJustify::Left)
		 ]
	 
	 
	     ]
		   //Load Button 
		  + SVerticalBox::Slot()
		  .Padding(ButtonPadding)
		 [

	       SNew(SButton)
	      .OnClicked(this, &SMainMenuWidget::OnLoadClicked)
		
		 [
		   SNew(STextBlock)
	      .Font(TitleTextStyle)
		  .Text(LoadText)
		  .Justification(ETextJustify::Left) 
		 ]
		  
		 ]
	 
		  //Quit Button 
		  + SVerticalBox::Slot()
		  .Padding(ButtonPadding)
         [

		   SNew(SButton)
		  .OnClicked(this, &SMainMenuWidget::OnQuitClicked)
			   
	     [
		   SNew(STextBlock)
		  .Font(TitleTextStyle)
	      .Text(QuitText)
	      .Justification(ETextJustify::Left)

		 ]
	 
		 ]   
		 
		 ]

	 ];		 
	
	    // Enable clicking events and show mouse cursor
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
		}
	
}

// When the play button is pressed. Remove the menu and hide it
FReply SMainMenuWidget::OnPlayClicked() const
{
	if (OwningHUD.IsValid()) 
	{
		OwningHUD->HideMenu(); 
	}
	
	return FReply::Handled();
}

// When the quit button is pressed. leave the menu and game
FReply SMainMenuWidget::OnQuitClicked() const
{
	if (OwningHUD.IsValid()) 
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)

			PC->ConsoleCommand("Quit");

	}

	return FReply::Handled();
}

// When the Load button is pressed. Remove the menu and load game. Thats when I get it working
FReply SMainMenuWidget::OnLoadClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->HideMenu();

	}

	return FReply::Handled();
}






#undef LOCTEXT_NAMESPACE

