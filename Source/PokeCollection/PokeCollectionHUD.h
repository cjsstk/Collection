// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/InGameMainWidget.h"
#include "PokeCollectionHUD.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API APokeCollectionHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void OpenInGameAdventureWidget();

	class UInGameMainWidget* GetInGameMainWidget() const { return InGameMainWidget; }
	class UInGameAdventureWidget* GetInGameAdventureWidget() const { return InGameAdventureWidget; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameMainWidget> InGameMainWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameAdventureWidget> InGameAdventureWidgetClass;

	UPROPERTY(Transient)
	class UInGameMainWidget* InGameMainWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameAdventureWidget* InGameAdventureWidget = nullptr;

};
