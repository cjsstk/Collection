// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCollectionHUD.h"

#include "Widgets/InGameAdventureWidget.h"
#include "Widgets/BattleStageInfoPopUp.h"

#include "Blueprint/UserWidget.h"
#include "WidgetLayoutLibrary.h"

void APokeCollectionHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InGameTopStatusBarClass.Get())
	{
		InGameTopStatusBar = CreateWidget<UInGameTopStatusBar>(GetWorld(), InGameTopStatusBarClass, FName("InGameTopStatusBar"));
		if (ensure(InGameTopStatusBar))
		{
			InGameTopStatusBar->AddToViewport(1);
		}
	}

	if (InGameMainWidgetClass.Get())
	{
		InGameMainWidget = CreateWidget<UInGameMainWidget>(GetWorld(), InGameMainWidgetClass, FName("InGameMainWidget"));
		if (ensure(InGameMainWidget))
		{
			InGameMainWidget->AddToViewport();
		}
	}

	if (InGameAdventureWidgetClass.Get())
	{
		InGameAdventureWidget = CreateWidget<UInGameAdventureWidget>(GetWorld(), InGameAdventureWidgetClass, FName("InGameAdventureWidget"));
		InGameAdventureWidget->SetPrevWidget(InGameMainWidget);
	}

	if (BattleStageInfoPopUpClass.Get())
	{
		BattleStageInfoPopUp = CreateWidget<UBattleStageInfoPopUp>(GetWorld(), BattleStageInfoPopUpClass, FName("BattleStageInfoPopUp"));
	}
}

void APokeCollectionHUD::OpenInGameAdventureWidget()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	if (ensure(InGameTopStatusBar))
	{
		InGameTopStatusBar->AddToViewport(1);
	}

	if (ensure(InGameAdventureWidget))
	{
		InGameAdventureWidget->AddToViewport();
		InGameAdventureWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenBattleStageInfoPopUp()
{

}
