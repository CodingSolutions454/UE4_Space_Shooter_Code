// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	// New game button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* NewGameButton;

	// Load Game Button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* LoadGameButton;

	// Settings game button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* SettingsGameButton;

	// Credits game button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* CreditsGameButton;

	// Quit game button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;

	virtual void NativeConstruct() override;

	// New Game Button OnClicked event
	UFUNCTION()
	void NewGameOnClicked();

	// Load Game Button OnClicked event
	UFUNCTION()
	void LoadGameOnClicked();

	// Settings Game Button OnClicked event
	UFUNCTION()
	void SettingsGameOnClicked();

	// Credits Game Button OnClicked event
	UFUNCTION()
	void CreditsGameOnClicked();
	// Quit game event
	UFUNCTION()
	void QuitOnClicked();
};
