// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "Widgets/CharacterBoxWidget.h"
#include "InGameCharacterBoxWidget.generated.h"

/** Character slot on making party */
UCLASS()
class POKECOLLECTION_API UCharacterBoxSlot : public UCharacterSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void OnSelectButtonClicked() override;

	void SetIsMakingParty(bool bInIsMakingParty) { bIsMakingParty = bInIsMakingParty; };

private:
	bool bIsMakingParty = false;
};


/**
 * Character Box Widget
 */
UCLASS()
class POKECOLLECTION_API UInGameCharacterBoxWidget : public UInGameWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

	void SetIsMakingParty(bool bInIsMakingParty) { bIsMakingParty = bInIsMakingParty; };

	void SetSelectedPartySlotNum(int32 InSelectedPartySlotNum) { SelectedPartySlotNum = InSelectedPartySlotNum; }
	int32 GetSelectedPartySlotNum() const { return SelectedPartySlotNum; }

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CharacterScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* CharacterGridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterBoxSlot> CharacterBoxSlotClass;

	int32 ColumnNum = 7;
	
	/** Make party */
	bool bIsMakingParty = false;

	int32 SelectedPartySlotNum = 0;
};
