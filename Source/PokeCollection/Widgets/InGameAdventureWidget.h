// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameAdventureWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UStageWidget : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Background;

};

/**
 * Adventure widget. Contains stage widgets
 */
UCLASS()
class POKECOLLECTION_API UInGameAdventureWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* StageWidgets;

};
