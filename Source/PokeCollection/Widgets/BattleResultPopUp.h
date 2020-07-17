// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "BattleResultPopUp.generated.h"

UCLASS()
class POKECOLLECTION_API UBattleRewardSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitSlot(const ERewardSlotType& InRewardType, int32 InRewardKey, int32 InRewardNum);

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* RewardImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RewardNumText = nullptr;
};

/**
 * ���� ������ ����� ��Ÿ���� �˾� ����
 */
UCLASS()
class POKECOLLECTION_API UBattleResultPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitResultInfo(FBattleReward& InBattleReward);

	UFUNCTION()
	void OnBackgroundButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* RewardBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBattleRewardSlot> RewardSlotClass = nullptr;

	FBattleReward BattleReward;

};

/**
 * ���� �й� �˾�
 */
UCLASS()
class POKECOLLECTION_API UBattleLosePopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackgroundButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

};