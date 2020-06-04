// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameTopStatusBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButtonClicked);

UCLASS(Blueprintable)
class POKECOLLECTION_API UTopStatusPlayerInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show(bool bInShow);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NicknameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ExpBar = nullptr;

	bool bShowing = false;
};


/**
 * 게임 상단에서 유저 정보를 나타내는 위젯
 */
UCLASS(Blueprintable)
class POKECOLLECTION_API UInGameTopStatusBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnBack();

	FOnBackButtonClicked OnBackButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BerryAmountText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MoneyAmountText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StardustAmountText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BerryChargingText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MoneyChargingText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackButtonImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTopStatusPlayerInfo* TopPlayerInfo = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* BackButtonTexture = nullptr;

};
