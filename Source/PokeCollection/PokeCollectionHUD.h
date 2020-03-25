// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/InGameMainWidget.h"
#include "CMS.h"
#include "PokeCollectionHUD.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API APokeCollectionHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void OpenInGameBoxWidget();
	void OpenInGameCharacterBoxWidget(bool bIsMakingParty);
	void OpenInGameCharacterInfoWidget(int32 InCharacterID);
	void OpenInGameShopWidget();
	void OpenInGameAdventureWidget();
	//void OpenInGameProfileWidget();
	void OpenInGameMakePartyWidget(bool bJustBeforeBattle);
	void OpenBattleStageInfoPopUp(battleStageKey InBattleStageKey);
	void OpenEquipmentInfoPopUp(int32 InEquipmentID);

	UFUNCTION()
	void OnStartBattle();

	void OnBackButtonClicked(class UInGameWidget* CurrentWidget);

	class UInGameMainWidget* GetInGameMainWidget() const { return InGameMainWidget; }
	class UInGameProfileWidget* GetInGameProfileWidget() const { return InGameProfileWidget; }
	class UInGameBoxWidget* GetInGameBoxWidget() const { return InGameBoxWidget; }
	class UInGameShopWidget* GetInGameShopWidget() const { return InGameShopWidget; }
	class UInGameAdventureWidget* GetInGameAdventureWidget() const { return InGameAdventureWidget; }
	class UInGameTopStatusBar* GetInGameTopStatusBar() const { return InGameTopStatusBar; }
	class UInGameMakePartyWidget* GetInGameMakePartyWidget() const { return InGameMakePartyWidget; }
	class UInGameCharacterBoxWidget* GetInGameCharacterBoxWidget() const { return InGameCharacterBoxWidget; }
	class UInGameCharacterInfoWidget* GetInGameCharacterInfoWidget() const { return InGameCharacterInfoWidget; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
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
	TSubclassOf<class UInGameShopWidget> InGameShopWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameAdventureWidget> InGameAdventureWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameTopStatusBar> InGameTopStatusBarClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameMakePartyWidget> InGameMakePartyWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBattleStageInfoPopUp> BattleStageInfoPopUpClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UEquipmentInfoPopUp> EquipmentInfoPopUpClass;

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
	class UInGameShopWidget* InGameShopWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameAdventureWidget* InGameAdventureWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameTopStatusBar* InGameTopStatusBar = nullptr;

	UPROPERTY(Transient)
	class UInGameMakePartyWidget* InGameMakePartyWidget = nullptr;

	UPROPERTY(Transient)
	class UBattleStageInfoPopUp* BattleStageInfoPopUp = nullptr;

	UPROPERTY(Transient)
	class UEquipmentInfoPopUp* EquipmentInfoPopUp = nullptr;

};
