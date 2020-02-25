// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameMakePartyWidget.generated.h"

/**
 * Party making widget
 */
UCLASS()
class POKECOLLECTION_API UInGameMakePartyWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	void SetJustBeforeBattle(bool bNewJustBeforeBattle) { bJustBeforeBattle = bNewJustBeforeBattle; }


private:

	bool bJustBeforeBattle = false;

};
