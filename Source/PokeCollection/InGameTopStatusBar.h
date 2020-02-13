// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameTopStatusBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButtonClicked);

/**
 * 
 */
UCLASS(Blueprintable)
class POKECOLLECTION_API UInGameTopStatusBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBack();

	FOnBackButtonClicked OnBackButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton = nullptr;
};
