// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillUpgradePopUp.generated.h"

/**
 * 스킬 업그레이드 팝업
 */
UCLASS()
class POKECOLLECTION_API USkillUpgradePopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitInfo(int32 InSkillKey, int32 InSkillIndex);

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
	class UImage* SkillImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* NeedItemBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NeedMoneyText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* UpgradeButton = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UNeedItemSlot> NeedItemSlotClass;

	UPROPERTY(Transient)
	TArray<class UNeedItemSlot*> NeedItemSlots;

	int32 NeedMoney = 0;
	int32 SkillIndex = -1;

	bool bUpgradeEnable = false;
};
