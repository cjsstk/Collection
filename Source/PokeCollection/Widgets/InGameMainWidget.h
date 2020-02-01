// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMainWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class POKECOLLECTION_API UInGameMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UFUNCTION()
	void OnStartButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* TempStartButton = nullptr;

};
