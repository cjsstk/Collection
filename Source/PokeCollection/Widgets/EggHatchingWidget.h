// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "Widgets/InGameMainWidget.h"
#include "EggHatchingWidget.generated.h"



UCLASS()
class POKECOLLECTION_API UEggHatchingWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void StartHatching(characterKey InCharacterKey);

	UFUNCTION()
	void OnSkipButtonClicked();

	UFUNCTION()
	void OnBackgroundButtonClicked();

	UFUNCTION()
	void OnHatchingAnimationFinished();

	FWidgetAnimationDynamicEvent OnAnimationFinished;

private:
	void CloseWidget();
	void StartAnim();

	UPROPERTY(meta = (BindWidget))
	class UImage* HatchingSprite = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* Background = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SkipButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HatchingText = nullptr;

	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* HatchingAnim = nullptr;

};
