// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MakePartySlot.generated.h"

UCLASS()
class POKECOLLECTION_API UMakePartySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void RefreshSlot();

	void SetSlotNum(int32 InSlotNum) { SlotNum = InSlotNum; }

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SlotImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* SlotCharacterInfoBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UCharacterSlot* SlotCharacter = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotCharacterHealth = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotCharacterAttack = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotCharacterDefense = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotCharacterSpAttack = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotCharacterSpDefense = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotCharacterSpeed = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotCharacterConsume = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SlotNum = -1;

	bool bIsEmptySlot = false;
};
