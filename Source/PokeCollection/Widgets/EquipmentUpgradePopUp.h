// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ItemBoxWidget.h"
#include "EquipmentUpgradePopUp.generated.h"

UCLASS()
class POKECOLLECTION_API UNeedItemSlot : public UItemSlot
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void InitByKey(int32 InContentKey) override;

	bool IsEnoughAmount() const { return bEnoughItem; }
	void SetNeedAmount(int32 InNeedAmount);

protected:
	void SetNeedAmountText(const FText& InNeedAmount);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NeedText = nullptr;

	int32 HaveAmount = 0;
	int32 NeedAmount = 0;
	bool bEnoughItem = false;
};


/**
 * 장비 업그레이드 팝업
 */
UCLASS()
class POKECOLLECTION_API UEquipmentUpgradePopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitInfo(int32 InEquipmentID);

	UFUNCTION()
	void OnBackgroundClicked();

	UFUNCTION()
	void OnUpgradeButtonClicked();

private:
	void SetNeedMoneyText(int32 InNeedMoney);
	void TickCheckUpgradeEnable();

	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* EquipmentImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EquipmentName = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* NeedItemBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NeedMoneyText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* UpgradeButton = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNeedItemSlot> NeedItemSlotClass;

	UPROPERTY(Transient)
	TArray<UNeedItemSlot*> NeedItemSlots;

	int32 NeedMoney = 0;
	int32 EquipmentID = -1;

	bool bUpgradeEnable = false;
};
