// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "Widgets/CharacterBoxWidget.h"
#include "InGameIndexWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UCharacterIndexSlot : public UCharacterSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() override;

	void Init(int32 InCharacterKey);
	void RefreshSlot();


private:
	/*UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* SlotMaterial = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* SlotMaterialInstance = nullptr;*/

	bool bEnabled = false;
};


/**
 * 캐릭터 도감 위젯
 */
UCLASS()
class POKECOLLECTION_API UInGameIndexWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

private:
	void Refresh();

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CharacterScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* CharacterGridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterIndexSlot> CharacterIndexSlotClass;

	int32 ColumnNum = 7;

};
