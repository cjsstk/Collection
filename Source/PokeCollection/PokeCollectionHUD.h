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
	void OpenInGameAdventureWidget();
	void OpenBattleStageInfoPopUp(battleStageKey InBattleStageKey);

	void OnBackButtonClicked(class UInGameWidget* CurrentWidget);

	class UInGameMainWidget* GetInGameMainWidget() const { return InGameMainWidget; }
	class UInGameAdventureWidget* GetInGameAdventureWidget() const { return InGameAdventureWidget; }
	class UInGameTopStatusBar* GetInGameTopStatusBar() const { return InGameTopStatusBar; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameMainWidget> InGameMainWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameAdventureWidget> InGameAdventureWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInGameTopStatusBar> InGameTopStatusBarClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBattleStageInfoPopUp> BattleStageInfoPopUpClass;

	UPROPERTY(Transient)
	class UInGameMainWidget* InGameMainWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameAdventureWidget* InGameAdventureWidget = nullptr;

	UPROPERTY(Transient)
	class UInGameTopStatusBar* InGameTopStatusBar = nullptr;

	UPROPERTY(Transient)
	class UBattleStageInfoPopUp* BattleStageInfoPopUp = nullptr;

};
