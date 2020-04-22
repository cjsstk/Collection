// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "BattleResultPopUp.generated.h"


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
