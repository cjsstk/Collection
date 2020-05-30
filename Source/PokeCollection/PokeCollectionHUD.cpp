// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCollectionHUD.h"

#include "BattleManager.h"
#include "PokeCollectionCharacter.h"
#include "PokeCore.h"
#include "Widgets/BattleResultPopUp.h"
#include "Widgets/BattleStageInfoPopUp.h"
#include "Widgets/BuyConfirmPopUp.h"
#include "Widgets/ChangeEquipmentInfoPopUp.h"
#include "Widgets/EggHatchingWidget.h"
#include "Widgets/EquipmentInfoPopUp.h"
#include "Widgets/InGameAdventureWidget.h"
#include "Widgets/InGameBattleWidget.h"
#include "Widgets/InGameBoxWidget.h"
#include "Widgets/InGameCharacterBoxWidget.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "Widgets/InGameChangeEquipmentWidget.h"
#include "Widgets/InGameDialogWidget.h"
#include "Widgets/InGameIndexWidget.h"
#include "Widgets/InGameMakePartyWidget.h"
#include "Widgets/InGameProfileWidget.h"
#include "Widgets/InGameShopWidget.h"
#include "Widgets/InGameSummonWidget.h"
#include "Widgets/LoginWidget.h"

#include "Blueprint/UserWidget.h"
#include "WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<int32> CVarShowInViewportWidgetsDebug
(
	TEXT("poke.showInViewportWidgetsDebug"),
	0,
	TEXT("1: Enable debug, 0: Disable debug")
);

void APokeCollectionHUD::BeginPlay()
{
	Super::BeginPlay();

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (LevelName == FString("LoginLevel"))
	{
		if (LoginWidgetClass.Get())
		{
			LoginWidget = CreateWidget<ULoginWidget>(GetWorld(), LoginWidgetClass, FName("LoginWidget"));
			ensure(LoginWidget);
		}
	}
	else if (LevelName == FString("InGameLevel"))
	{
		if (LoginWidget)
		{
			LoginWidget->BeginDestroy();
			LoginWidget = nullptr;
		}

		if (InGameTopStatusBarClass.Get())
		{
			InGameTopStatusBar = CreateWidget<UInGameTopStatusBar>(GetWorld(), InGameTopStatusBarClass, FName("InGameTopStatusBar"));
			ensure(InGameTopStatusBar);
		}

		if (InGameMainWidgetClass.Get())
		{
			InGameMainWidget = CreateWidget<UInGameMainWidget>(GetWorld(), InGameMainWidgetClass, FName("InGameMainWidget"));
			ensure(InGameMainWidget);
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

		if (InGameChangeEquipmentWidgetClass.Get())
		{
			InGameChangeEquipmentWidget = CreateWidget<UInGameChangeEquipmentWidget>(GetWorld(), InGameChangeEquipmentWidgetClass, FName("InGameChangeEquipmentWidget"));
			if (ensure(InGameChangeEquipmentWidget))
			{
				InGameChangeEquipmentWidget->SetPrevWidget(InGameCharacterInfoWidget);
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
			if (EggHatchingWidget)
			{
				EggHatchingWidget->SetPrevWidget(InGameShopWidget);
			}
		}

		if (ChangeEquipmentInfoPopUpClass.Get())
		{
			ChangeEquipmentInfoPopUp = CreateWidget<UChangeEquipmentInfoPopUp>(GetWorld(), ChangeEquipmentInfoPopUpClass, FName("ChangeEquipmentInfoPopUp"));
		}

		if (InGameIndexWidgetClass.Get())
		{
			InGameIndexWidget = CreateWidget<UInGameIndexWidget>(GetWorld(), InGameIndexWidgetClass, FName("InGameIndexWidget"));
			if (ensure(InGameIndexWidget))
			{
				InGameIndexWidget->SetPrevWidget(InGameMainWidget);
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

		if (InGameBattleWidgetClass.Get())
		{
			InGameBattleWidget = CreateWidget<UInGameBattleWidget>(GetWorld(), InGameBattleWidgetClass, FName("InGameBattleWidget"));
		}

		if (InGameSummonWidgetClass.Get())
		{
			InGameSummonWidget = CreateWidget<UInGameSummonWidget>(GetWorld(), InGameSummonWidgetClass, FName("InGameSummonWidget"));
			if (ensure(InGameSummonWidget))
			{
				InGameSummonWidget->SetPrevWidget(InGameMainWidget);
			}
		}
	}

	OnWidgetLoaded.Broadcast();
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

void APokeCollectionHUD::OpenInGameCharacterBoxWidget(bool bIsEmpySlot)
{
	if (ensure(InGameMakePartyWidget))
	{
		InGameMakePartyWidget->RemoveFromViewport();
		InGameCharacterBoxWidget->SetPrevWidget(InGameMakePartyWidget);
	}

	if (ensure(InGameCharacterBoxWidget))
	{
		InGameCharacterBoxWidget->SetEmptySlotSelected(bIsEmpySlot);
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
		InGameCharacterInfoWidget->SwitchContentWidget(0);
		InGameCharacterInfoWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenInGameChangeEquipmentWidget()
{
	if (ensure(InGameCharacterInfoWidget))
	{
		InGameCharacterInfoWidget->RemoveFromViewport();
	}

	if (ensure(InGameChangeEquipmentWidget))
	{
		InGameChangeEquipmentWidget->AddToViewport();
		InGameChangeEquipmentWidget->OnOpen();
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
	if (ensure(InGameShopWidget))
	{
		InGameShopWidget->RemoveFromViewport();
	}

	if (ensure(InGameTopStatusBar))
	{
		InGameTopStatusBar->RemoveFromViewport();
	}

	if (ensure(EggHatchingWidget))
	{
		EggHatchingWidget->StartHatching(NewCharacterKey);
		EggHatchingWidget->AddToViewport();
	}
}

void APokeCollectionHUD::OpenInGameIndexWidget()
{
	if (ensure(InGameMainWidget))
	{
		InGameMainWidget->RemoveFromViewport();
	}

	if (ensure(InGameIndexWidget))
	{
		InGameIndexWidget->AddToViewport();
		InGameIndexWidget->OnOpen();
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

void APokeCollectionHUD::OpenInGameBattleWidget(const TArray<class ABattleCharacterActor*>& InPlayerBattleCharacter)
{
	if (ensure(InGameBattleWidget))
	{
		InGameBattleWidget->AddToViewport();
		InGameBattleWidget->InitBattleCharacters(InPlayerBattleCharacter);
	}
}

void APokeCollectionHUD::OpenBattleResultPopUp(FBattleReward& InBattleReward)
{
	if (!BattleResultPopUpClass.Get())
	{
		return;
	}

	UBattleResultPopUp* BattleResultPopUp = CreateWidget<UBattleResultPopUp>(GetWorld(), BattleResultPopUpClass, FName("BattleResultPopUp"));
	if (ensure(BattleResultPopUp))
	{
		BattleResultPopUp->AddToViewport(1);
		BattleResultPopUp->InitResultInfo(InBattleReward);
	}
}

void APokeCollectionHUD::OpenBattleLosePopUp()
{
	UBattleLosePopUp* BattleLosePopUp = CreateWidget<UBattleLosePopUp>(GetWorld(), BattleLosePopUpClass, FName("BattleLosePopUp"));
	if (ensure(BattleLosePopUp))
	{
		BattleLosePopUp->AddToViewport(2);
	}
}

void APokeCollectionHUD::OpenEquipmentInfoPopUp(int32 InEquipmentID)
{
	if (!EquipmentInfoPopUpClass.Get())
	{
		return;
	}

	UEquipmentInfoPopUp* EquipmentInfoPopUp = CreateWidget<UEquipmentInfoPopUp>(GetWorld(), EquipmentInfoPopUpClass, FName("EquipmentInfoPopUp"));
	if (ensure(EquipmentInfoPopUp))
	{
		EquipmentInfoPopUp->AddToViewport(2);
		EquipmentInfoPopUp->InitInfo(InEquipmentID);
	}
}

void APokeCollectionHUD::OpenChangeEquipmentInfoPopUp(int32 InNextEquipmentID)
{
	if (ensure(ChangeEquipmentInfoPopUp))
	{
		ChangeEquipmentInfoPopUp->AddToViewport(2);
		ChangeEquipmentInfoPopUp->InitInfo(InNextEquipmentID);
	}
}

void APokeCollectionHUD::OpenInGameSummonWidget()
{
	if (ensure(InGameMainWidget))
	{
		InGameMainWidget->RemoveFromViewport();
	}

	if (ensure(InGameSummonWidget))
	{
		InGameSummonWidget->AddToViewport();
		InGameSummonWidget->OnOpen();
	}
}

void APokeCollectionHUD::OpenDialogWidget(int32 InDialogKey)
{
	if (InGameDialogWidgetClass.Get())
	{
		UInGameDialogWidget* DialogWidget = CreateWidget<UInGameDialogWidget>(GetWorld(), InGameDialogWidgetClass, FName("DialogWidget"));
		if (DialogWidget)
		{
			DialogWidget->AddToViewport(1);
			DialogWidget->InitDialog(InDialogKey);
		}
	}

	if (InGameTopStatusBar)
	{
		InGameTopStatusBar->RemoveFromViewport();
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

void APokeCollectionHUD::OnShutdownBattle()
{
	if (InGameBattleWidget)
	{
		InGameBattleWidget->RemoveFromViewport();
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
