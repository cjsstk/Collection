// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameMakePartyWidget.generated.h"

/**
 * Party making widget
 */
UCLASS()
class POKECOLLECTION_API UInGameMakePartyWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;
	virtual void OnBack() override;

	void RefreshSlots();

	UFUNCTION()
	void OnDecisionButtonClicked();

	UFUNCTION()
	void OnParty1ButtonClicked();

	UFUNCTION()
	void OnParty2ButtonClicked();

	UFUNCTION()
	void OnParty3ButtonClicked();

	UFUNCTION()
	void OnParty4ButtonClicked();

	void SetJustBeforeBattle(bool bNewJustBeforeBattle) { bJustBeforeBattle = bNewJustBeforeBattle; }

private:
	bool CanStartBattle();

	UPROPERTY(meta = (BindWidget))
	class UButton* DecisionButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* Party1Button = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* Party2Button = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* Party3Button = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* Party4Button = nullptr;

	UPROPERTY(Transient)
	TArray<class AMakePartyCharacterPanel*> CharacterSlots;

	bool bJustBeforeBattle = false;

};
