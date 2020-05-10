// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/InGameMainWidget.h"
#include "CMS.h"
#include "PokeCollectionHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetLoaded);

UCLASS()
class POKECOLLECTION_API APokeCollectionHUD : public AHUD
{
	GENERATED_BODY()

public:
	void OpenInGameBoxWidget();
	void OpenInGameCharacterBoxWidget(bool bIsEmpySlot);
	void OpenInGameCharacterInfoWidget(int32 InCharacterID);
	void OpenInGameChangeEquipmentWidget();
	void OpenInGameShopWidget();
	void OpenBuyConfirmPopUp(int32 InSlotKey, EShopSlotType InSlotType);
	void OpenEggHatchingWidget(characterKey NewCharacterKey);
	void OpenInGameIndexWidget();
	void OpenInGameAdventureWidget();
	//void OpenInGameProfileWidget();
	void OpenInGameMakePartyWidget(bool bJustBeforeBattle);
	void OpenBattleStageInfoPopUp(battleStageKey InBattleStageKey);
	void OpenInGameBattleWidget(const TArray<class ABattleCharacterActor*>& InPlayerBattleCharacter);
	void OpenBattleResultPopUp(FBattleReward& InBattleReward);
	void OpenEquipmentInfoPopUp(int32 InEquipmentID);

	void OpenDialogWidget(int32 InDialogKey);

	UFUNCTION()
	void OnStartBattle();

	UFUNCTION()
	void OnShutdownBattle();

	void OnBackButtonClicked(class UInGameWidget* CurrentWidget);

	class ULoginWidget* GetLoginWidget() const { return LoginWidget; }
	class UInGameMainWidget* GetInGameMainWidget() const { return InGameMainWidget; }
	class UInGameTopStatusBar* GetInGameTopStatusBar() const { return InGameTopStatusBar; }
	class UInGameProfileWidget* GetInGameProfileWidget() const { return InGameProfileWidget; }
	class UInGameBoxWidget* GetInGameBoxWidget() const { return InGameBoxWidget; }
	class UInGameShopWidget* GetInGameShopWidget() const { return InGameShopWidget; }
	class UBuyConfirmPopUp* GetBuyConfirmPopUp() const { return BuyConfirmPopUp; }
	class UEggHatchingWidget* GetEggHatchingWidget() const { return EggHatchingWidget; }
	class UInGameAdventureWidget* GetInGameAdventureWidget() const { return InGameAdventureWidget; }
	class UInGameMakePartyWidget* GetInGameMakePartyWidget() const { return InGameMakePartyWidget; }
	class UInGameCharacterBoxWidget* GetInGameCharacterBoxWidget() const { return InGameCharacterBoxWidget; }
	class UInGameCharacterInfoWidget* GetInGameCharacterInfoWidget() const { return InGameCharacterInfoWidget; }

	FOnWidgetLoaded OnWidgetLoaded;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULoginWidget> LoginWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameMainWidget> InGameMainWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameProfileWidget> InGameProfileWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameBoxWidget> InGameBoxWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameCharacterBoxWidget> InGameCharacterBoxWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameCharacterInfoWidget> InGameCharacterInfoWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameChangeEquipmentWidget> InGameChangeEquipmentWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameShopWidget> InGameShopWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBuyConfirmPopUp> BuyConfirmPopUpClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UEggHatchingWidget> EggHatchingWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameIndexWidget> InGameIndexWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameAdventureWidget> InGameAdventureWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameTopStatusBar> InGameTopStatusBarClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameMakePartyWidget> InGameMakePartyWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBattleStageInfoPopUp> BattleStageInfoPopUpClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameBattleWidget> InGameBattleWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBattleResultPopUp> BattleResultPopUpClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UEquipmentInfoPopUp> EquipmentInfoPopUpClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameDialogWidget> InGameDialogWidgetClass;

	//
	UPROPERTY(Transient)
	class ULoginWidget* LoginWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameMainWidget* InGameMainWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameProfileWidget* InGameProfileWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameBoxWidget* InGameBoxWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameCharacterBoxWidget* InGameCharacterBoxWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameCharacterInfoWidget* InGameCharacterInfoWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameChangeEquipmentWidget* InGameChangeEquipmentWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameShopWidget* InGameShopWidget = nullptr;

	UPROPERTY(Transient)
	class UBuyConfirmPopUp* BuyConfirmPopUp = nullptr;

	UPROPERTY(Transient)
	class UEggHatchingWidget* EggHatchingWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameIndexWidget* InGameIndexWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameAdventureWidget* InGameAdventureWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameTopStatusBar* InGameTopStatusBar = nullptr;

	UPROPERTY(Transient)
	class UInGameMakePartyWidget* InGameMakePartyWidget = nullptr;

	UPROPERTY(Transient)
	class UBattleStageInfoPopUp* BattleStageInfoPopUp = nullptr;

	UPROPERTY(Transient)
	class UInGameBattleWidget* InGameBattleWidget = nullptr;

	UPROPERTY(Transient)
	class UBattleResultPopUp* BattleResultPopUp = nullptr;

	UPROPERTY(Transient)
	class UEquipmentInfoPopUp* EquipmentInfoPopUp = nullptr;

};
