// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "BuyConfirmPopUp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuyButtonClicked, int32, InSlotKey);

UCLASS()
class POKECOLLECTION_API UBuyConfirmPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitText(int32 InSlotKey, EShopSlotType InSlotType);

	UFUNCTION()
	void OnBackgroundClicked();

	UFUNCTION()
	void OnBuyButtonClick();

	FOnBuyButtonClicked OnBuyButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BuyConfirmText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* BuyButton = nullptr;

	int32 SelectedSlotKey = 0;

};
