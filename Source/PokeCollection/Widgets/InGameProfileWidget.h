// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameProfileWidget.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UInGameProfileWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnOpen() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNickname = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerLevel = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerCurrentExp = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerMaxExp = nullptr;

};
