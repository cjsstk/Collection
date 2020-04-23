// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "InGameDialogWidget.generated.h"



/**
 * Dialog widget
 */
UCLASS()
class POKECOLLECTION_API UInGameDialogWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitDialog(int32 InDialogKey);

	UFUNCTION()
	void OnBackgroundButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TalkPersonName = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogText = nullptr;

	TArray<FDialogTalk> DialogTalk;

	int32 CurrentDialogIndex = 0;

	int32 CurrentTextChopNum = 0;
	int32 CurrentTextLen = 0;

	float DialogTextDelaySeconds = 0.1f;
	float DialogTextDelayAgeSeconds = 0.0f;
};
