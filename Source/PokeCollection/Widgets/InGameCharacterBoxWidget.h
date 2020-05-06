// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "Widgets/CharacterBoxWidget.h"
#include "InGameCharacterBoxWidget.generated.h"

/** 편성 할 때 나타나는 캐릭터 슬롯 */
UCLASS()
class POKECOLLECTION_API UCharacterBoxSlot : public UCharacterSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() override;

private:

};

/** 편성할 때 캐릭터를 파티에서 제외 시키는 슬롯 */
UCLASS()
class POKECOLLECTION_API UExcludeCharacterSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnExcludeButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ExcludeButton = nullptr;
};


/**
 * 캐릭터 편성 할 때 캐릭터를 고르는 화면 위젯
 */
UCLASS()
class POKECOLLECTION_API UInGameCharacterBoxWidget : public UInGameWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

	void SetEmptySlotSelected(bool bInIsEmptySlot) { bSelectEmptySlot = bInIsEmptySlot; };

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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UExcludeCharacterSlot> ExcludeCharacterSlotClass;

	int32 ColumnNum = 7;
	
	/** Make party */
	bool bIsMakingParty = true;

	bool bSelectEmptySlot = false;

	int32 SelectedPartySlotNum = 0;
};
