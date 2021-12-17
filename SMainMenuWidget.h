// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Slate.h"


/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
 
public:
	
	// PTR to main menu
	SLATE_BEGIN_ARGS(SMainMenuWidget) {}
	
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS() 

	/** every widget needs a construction function*/
	void Construct(const FArguments& InArgs);

	// OnClick events
	FReply OnPlayClicked() const;
	FReply OnQuitClicked() const;
	FReply OnLoadClicked() const;

	/**The HUD that created the widget*/
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	// Support keyboard focus
	virtual bool SupportsKeyboardFocus() const override { return true; };
	
};
