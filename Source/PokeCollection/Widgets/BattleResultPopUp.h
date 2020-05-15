// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "BattleResultPopUp.generated.h"

/**
 * 전투 끝나고 결과가 나타나는 팝업 위젯
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

};

/**
 * 전투 패배 팝업
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