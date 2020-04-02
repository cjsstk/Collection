// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCollectionHUD.h"

#include "Widgets/InGameAdventureWidget.h"
#include "Widgets/InGameBoxWidget.h"
#include "Widgets/BattleStageInfoPopUp.h"
#include "Widgets/EggHatchingWidget.h"
#include "Widgets/EquipmentInfoPopUp.h"
#include "Widgets/InGameCharacterBoxWidget.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "Widgets/InGameShopWidget.h"
#include "Widgets/BuyConfirmPopUp.h"
#include "Widgets/InGameProfileWidget.h"
#include "Widgets/InGameMakePartyWidget.h"
#include "PokeCollectionCharacter.h"
#include "PokeCore.h"
#include "BattleManager.h"

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

	if (InGameBoxWidgetClass.Get())
	{
		InGameBoxWidget = CreateWidget<UInGameBoxWidget>(GetWorld(), InGameBoxWidgetClass, FName("InGameBoxWidget"));
		if (ensure(InGameBoxWidget))
		{
			InGameBoxWidget->SetPrevWidget(InGameMainWidget);
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

	if (InGameCharacterInfoWidgetClass.Get())
	{
		InGameCharacterInfoWidget = CreateWidget<UInGameCharacterInfoWidget>(GetWorld(), InGameCharacterInfoWidgetClass, FName("InGameCharacterInfoWidget"));
		if (ensure(InGameCharacterInfoWidget))
		{
			InGameCharacterInfoWidget->SetPrevWidget(InGameBoxWidget);
		}
	}

	if (InGameShopWidgetClass.Get())
	{
		InGameShopWidget = CreateWidget<UInGameShopWidget>(GetWorld(), InGameShopWidgetClass, FName("InGameShopWidget"));
		if (ensure(InGameShopWidget))
		{
			InGameShopWidget->SetPrevWidget(InGameMainWidget);
		}
	}

	if (BuyConfirmPopUpClass.Get())
	{
		BuyConfirmPopUp = CreateWidget<UBuyConfirmPopUp>(GetWorld(), BuyConfirmPopUpClass, FName("BuyConfirmPopUp"));
	}

	if (EggHatchingWidgetClass.Get())
	{
		EggHatchingWidget = CreateWidget<UEggHatchingWidget>(GetWorld(), EggHatchingWidgetClass, FName("EggHatchingWidget"));
	}

	if (EquipmentInfoPopUpClass.Get())
	{
		EquipmentInfoPopUp = CreateWidget<UEquipmentInfoPopUp>(GetWorld(), EquipmentInfoPopUpClass, FName("EquipmentInfoPopUp"));
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

	ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
	if (ensure(BattleManager))
	{
		BattleManager->OnBattleStart.AddUniqueDynamic(this, &APokeCollectionHUD::OnStartBattle);
	}
}

void APokeCollectionHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APokeCollectionHUD::OpenInGameBoxWidget()
{
	if (ensure(InGameMainWidget))
	{
		InGameMainWidget->RemoveFromViewport();
	}

	if (ensure(InGameBoxWidget))
	{
		InGameBoxWidget->AddToViewport();
		InGameBoxWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenInGameCharacterBoxWidget(bool bIsMakingParty)
{
	if (bIsMakingParty)
	{
		if (ensure(InGameMakePartyWidget))
		{
			InGameMakePartyWidget->RemoveFromViewport();
			InGameCharacterBoxWidget->SetPrevWidget(InGameMakePartyWidget);
		}
	}
	else
	{
		if (ensure(InGameMainWidget))
		{
			InGameMainWidget->RemoveFromViewport();
			InGameCharacterBoxWidget->SetPrevWidget(InGameMainWidget);
		}
	}

	if (ensure(InGameCharacterBoxWidget))
	{
		InGameCharacterBoxWidget->SetIsMakingParty(bIsMakingParty);
		InGameCharacterBoxWidget->AddToViewport();
		InGameCharacterBoxWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenInGameCharacterInfoWidget(int32 InCharacterID)
{
	if (ensure(InGameBoxWidget))
	{
		InGameBoxWidget->RemoveFromViewport();
	}

	if (ensure(InGameCharacterInfoWidget))
	{
		InGameCharacterInfoWidget->AddToViewport();
		InGameCharacterInfoWidget->SetSelectedCharacterID(InCharacterID);
		InGameCharacterInfoWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenInGameShopWidget()
{
	if (ensure(InGameMainWidget))
	{
		InGameMainWidget->RemoveFromViewport();
	}

	if (ensure(InGameShopWidget))
	{
		InGameShopWidget->AddToViewport();
		InGameShopWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenBuyConfirmPopUp(int32 InSlotKey, EShopSlotType InSlotType)
{
	if (ensure(BuyConfirmPopUp))
	{
		BuyConfirmPopUp->InitText(InSlotKey, InSlotType);
		BuyConfirmPopUp->AddToViewport();
	}
}

void APokeCollectionHUD::OpenEggHatchingWidget(characterKey NewCharacterKey)
{
	if (ensure(EggHatchingWidget))
	{
		EggHatchingWidget->StartHatching(NewCharacterKey);
		EggHatchingWidget->AddToViewport();
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

void APokeCollectionHUD::OpenEquipmentInfoPopUp(int32 InEquipmentID)
{
	if (ensure(EquipmentInfoPopUp))
	{
		EquipmentInfoPopUp->AddToViewport(2);
		EquipmentInfoPopUp->InitInfo(InEquipmentID);
	}
}

void APokeCollectionHUD::OnStartBattle()
{
	if (InGameMakePartyWidget)
	{
		InGameMakePartyWidget->RemoveFromViewport();
	}

	if (InGameTopStatusBar)
	{
		InGameTopStatusBar->RemoveFromViewport();
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
