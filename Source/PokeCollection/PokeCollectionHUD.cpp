// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCollectionHUD.h"

#include "Widgets/InGameAdventureWidget.h"
#include "Widgets/BattleStageInfoPopUp.h"
#include "Widgets/InGameCharacterBoxWidget.h"
#include "Widgets/InGameProfileWidget.h"
#include "Widgets/InGameMakePartyWidget.h"

#include "Blueprint/UserWidget.h"
#include "WidgetLayoutLibrary.h"

static TAutoConsoleVariable<int32> CVarShowInViewportWidgetsDebug
(
	TEXT("poke.showInViewportWidgetsDebug"),
	0,
	TEXT("1: Enable debug, 0: Disable debug")
);

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

	if (InGameProfileWidgetClass.Get())
	{
		InGameProfileWidget = CreateWidget<UInGameProfileWidget>(GetWorld(), InGameProfileWidgetClass, FName("InGameProfileWidget"));
		if (ensure(InGameProfileWidget) && InGameMainWidget)
		{
			InGameProfileWidget->SetPrevWidget(InGameMainWidget);
			InGameMainWidget->SetPrevWidget(InGameProfileWidget);
		}
	}

	if (InGameCharacterBoxWidgetClass.Get())
	{
		InGameCharacterBoxWidget = CreateWidget<UInGameCharacterBoxWidget>(GetWorld(), InGameCharacterBoxWidgetClass, FName("InGameCharacterBoxWidget"));
		if (ensure(InGameCharacterBoxWidget))
		{
			InGameCharacterBoxWidget->SetPrevWidget(InGameMainWidget);
		}
	}

	if (InGameAdventureWidgetClass.Get())
	{
		InGameAdventureWidget = CreateWidget<UInGameAdventureWidget>(GetWorld(), InGameAdventureWidgetClass, FName("InGameAdventureWidget"));
		if (ensure(InGameAdventureWidget))
		{
			InGameAdventureWidget->SetPrevWidget(InGameMainWidget);
		}
	}

	if (InGameMakePartyWidgetClass.Get())
	{
		InGameMakePartyWidget = CreateWidget<UInGameMakePartyWidget>(GetWorld(), InGameMakePartyWidgetClass, FName("InGameMakePartyWidget"));
		if (ensure(InGameMakePartyWidget))
		{
			InGameMakePartyWidget->SetPrevWidget(InGameMainWidget);
		}
	}

	if (BattleStageInfoPopUpClass.Get())
	{
		BattleStageInfoPopUp = CreateWidget<UBattleStageInfoPopUp>(GetWorld(), BattleStageInfoPopUpClass, FName("BattleStageInfoPopUp"));
	}
}

void APokeCollectionHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APokeCollectionHUD::OpenInGameCharacterBoxWidget()
{
	if (ensure(InGameMainWidget))
	{
		InGameMainWidget->RemoveFromViewport();
	}

	if (ensure(InGameCharacterBoxWidget))
	{
		InGameCharacterBoxWidget->AddToViewport();
		InGameCharacterBoxWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenInGameAdventureWidget()
{
	//UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	if (ensure(InGameMainWidget))
	{
		InGameMainWidget->RemoveFromViewport();
	}

	if (ensure(InGameAdventureWidget))
	{
		InGameAdventureWidget->AddToViewport();
		InGameAdventureWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenInGameProfileWidget()
{
}

void APokeCollectionHUD::OpenInGameMakePartyWidget(bool bJustBeforeBattle)
{
	if (bJustBeforeBattle)
	{
		if (ensure(InGameAdventureWidget))
		{
			InGameAdventureWidget->RemoveFromViewport();
			InGameMakePartyWidget->SetPrevWidget(InGameAdventureWidget);
		}
	}
	else
	{
		if (ensure(InGameMainWidget))
		{
			InGameMainWidget->RemoveFromViewport();
			InGameMakePartyWidget->SetPrevWidget(InGameMainWidget);
		}
	}

	InGameMakePartyWidget->AddToViewport();
	InGameMakePartyWidget->SetJustBeforeBattle(bJustBeforeBattle);
	InGameMakePartyWidget->OnOpen();

}

void APokeCollectionHUD::OpenBattleStageInfoPopUp(battleStageKey InBattleStageKey)
{
	if (ensure(BattleStageInfoPopUp) && InBattleStageKey != INVALID_BATTLESTAGEKEY)
	{
		BattleStageInfoPopUp->AddToViewport(2);
		BattleStageInfoPopUp->InitInfo(InBattleStageKey);
	}
}

void APokeCollectionHUD::OnBackButtonClicked(class UInGameWidget* CurrentWidget)
{
	if (!CurrentWidget)
	{
		ensure(0);
		return;
	}

	CurrentWidget->RemoveFromViewport();

	if (CurrentWidget->GetPrevWidget() && !CurrentWidget->GetPrevWidget()->IsInViewport())
	{
		CurrentWidget->GetPrevWidget()->AddToViewport();
		CurrentWidget->GetPrevWidget()->OnOpen();
	}
	else
	{

	}
}
