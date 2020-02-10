// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameAdventureWidget.generated.h"


UCLASS(abstract)
class POKECOLLECTION_API UInGameFullScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnOpen() {};
};

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UInGameAdventureWidget : public UInGameFullScreenWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Background = nullptr;

};
