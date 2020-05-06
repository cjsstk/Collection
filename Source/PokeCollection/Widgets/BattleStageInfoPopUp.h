// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMS.h"
#include "BattleStageInfoPopUp.generated.h"

/** 
 * 모험 화면에 나타나는 전투 스테이지 간판 위젯
 */
UCLASS(Blueprintable)
class POKECOLLECTION_API UBattleStageSignboard : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSignboardClicked();


private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BattleStageName = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SignboardButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 BattleStageKey = 0;

};

/**
 * 전투 스테이지 간판을 클릭하면 나타나는 스테이지 정보 화면 위젯
 */
UCLASS()
class POKECOLLECTION_API UBattleStageInfoPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void InitInfo(battleStageKey InBattleStageKey);

	UFUNCTION()
	void OnBackgroundClicked();

	UFUNCTION()
	void OnStartBattleClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartBattleButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BattleStageName = nullptr;
};
