// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameBoxWidget.h"
#include "Widgets/CharacterBoxWidget.h"
#include "Widgets/EquipmentBoxWidget.h"
#include "DestoryContentWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDestroySlotClicked, bool, bSelected, int32, ContentID);

/** 분해 할 때 나타나는 캐릭터 슬롯 */
UCLASS()
class POKECOLLECTION_API UDestoryCharacterSlot : public UCharacterSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() override;
	virtual void InitByKey(int32 InContentKey) override;

	FOnDestroySlotClicked OnDestroySlotClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SelectedImage = nullptr;

	bool bSelected = false;

};

/** 분해 할 때 나타나는 장비 슬롯 */
UCLASS()
class POKECOLLECTION_API UDestoryEquipmentSlot : public UEquipmentSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() override;
	virtual void InitByKey(int32 InContentKey) override;

	FOnDestroySlotClicked OnDestroySlotClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SelectedImage = nullptr;

	bool bSelected = false;

};


/**
 * 캐릭터 분해 위젯
 */
UCLASS()
class POKECOLLECTION_API UDestoryContentWidget : public UBoxContentWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;
	virtual void RefreshSlot() override;

	UFUNCTION()
	void OnCharacterButtonClicked();

	UFUNCTION()
	void OnEquipmentButtonClicked();

	UFUNCTION()
	void OnDestroyButtonClicked();

	UFUNCTION()
	void OnDestroySlotSelected(bool bSelected, int32 InContentID);

private:
	void RefreshCharacter();
	void RefreshEquipment();

	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* EquipmentButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* DestroyButton = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "DestroyContent")
	TSubclassOf<class UBoxSlot> CharacterSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "DestroyContent")
	TSubclassOf<class UBoxSlot> EquipmentSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "DestroyContent")
	int32 CharacterSlotColumnNum = 5;

	UPROPERTY(EditDefaultsOnly, Category = "DestroyContent")
	int32 EquipmentSlotColumnNum = 7;
};
