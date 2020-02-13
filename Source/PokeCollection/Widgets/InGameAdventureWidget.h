// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameAdventureWidget.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UInGameAdventureWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnOpen() override;
	virtual void OnBack() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Background = nullptr;

};
