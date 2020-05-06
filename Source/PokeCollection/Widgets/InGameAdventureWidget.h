// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameAdventureWidget.generated.h"


/**
 * 모험 화면 위젯에 나타나는 스테이지 위젯
 */
UCLASS()
class POKECOLLECTION_API UStageWidget : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Background;

	UPROPERTY(meta = (BindWidget))
	class UBattleStageSignboard* BattleStageOne;

	UPROPERTY(meta = (BindWidget))
	class UBattleStageSignboard* BattleStageTwo;

	UPROPERTY(meta = (BindWidget))
	class UBattleStageSignboard* BattleStageThree;

	UPROPERTY(meta = (BindWidget))
	class UBattleStageSignboard* BattleStageFour;

};

/**
 * 모험 화면 위젯.
 */
UCLASS(Blueprintable)
class POKECOLLECTION_API UInGameAdventureWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

	UFUNCTION()
	void OnPrevStageButtonClicked();

	UFUNCTION()
	void OnNextStageButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* StageWidgets;

	UPROPERTY(meta = (BindWidget))
	class UButton* PrevStageButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextStageButton;
};
