// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "BuyConfirmPopUp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuyButtonClicked, int32, InSlotKey);

/**
 * 상점에서 구매 확인을 표시하는 팝업 위젯
 */
UCLASS()
class POKECOLLECTION_API UBuyConfirmPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	void InitText(int32 InSlotKey, EShopSlotType InSlotType);

	UFUNCTION()
	void OnBackgroundClicked();

	UFUNCTION()
	void OnBuyButtonClick();

	FOnBuyButtonClicked OnBuyButtonClicked;

private:
	void TickCheckBuyEnable();

	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BuyConfirmText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* BuyButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BuyButtonText = nullptr;

	int32 SelectedSlotKey = 0;

	FText SlotName;
	int32 SlotPrice = 0;
};
