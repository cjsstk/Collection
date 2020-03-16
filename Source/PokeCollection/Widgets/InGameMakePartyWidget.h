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

	void SetJustBeforeBattle(bool bNewJustBeforeBattle) { bJustBeforeBattle = bNewJustBeforeBattle; }

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* DecisionButton = nullptr;

	UPROPERTY(Transient)
	TArray<class AMakePartyCharacterPanel*> CharacterSlots;

	bool bJustBeforeBattle = false;

};
