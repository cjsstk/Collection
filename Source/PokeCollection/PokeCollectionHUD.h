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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameMainWidget> InGameMainWidgetClass;

	UPROPERTY(Transient)
	class UInGameMainWidget* InGameMainWidget = nullptr;

};
