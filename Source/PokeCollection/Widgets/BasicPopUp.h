// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMSType.h"
#include "BasicPopUp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmButtonClicked);


/**
 * ±âº» ÆË¾÷
 */
UCLASS()
class POKECOLLECTION_API UBasicPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnConfirmed() {}

	void SetConfirmText(FText InText);

	UFUNCTION()
	void OnBackgroundClicked();

	UFUNCTION()
	void OnConfirmButtonClicked();

	//FOnConfirmButtonClicked OnConfirmButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConfirmText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton = nullptr;

	//UPROPERTY(meta = (BindWidget))
	//class UTextBlock* BuyButtonText = nullptr;
};
