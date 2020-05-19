// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/InGameBoxWidget.h"
#include "TypeChart.h"
#include "CharacterBoxWidget.generated.h"

/**
 * 캐릭터 정보가 담긴 슬롯
 */
UCLASS()
class POKECOLLECTION_API UCharacterSlot : public UBoxSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() override;
	virtual void InitByID(int32 InContentID) override;
	virtual void InitByKey(int32 InContentKey) override;

	void SetContentLevel(int32 InLevel);
	void SetTypeImages(CharacterType InCharacterType);
	void SetJoinedPartyText(int32 InJoinedPartyNum);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Type1Image = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Type2Image = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* JoinedPartyText = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* TypeMaterial = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* Type1MaterialInstance = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* Type2MaterialInstance = nullptr;
};

/**
 * 박스에서 가진 캐릭터들이 나타나는 화면 위젯
 */
UCLASS()
class POKECOLLECTION_API UCharacterBoxWidget : public UBoxContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
	virtual void RefreshSlot() override;

private:

};
