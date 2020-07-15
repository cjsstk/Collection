// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UseItemPopUp.generated.h"

/**
 * 아이템 사용 시 나타나는 팝업
 */
UCLASS()
class POKECOLLECTION_API UUseItemPopUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void InitInfo(int32 InItemKey);

	UFUNCTION()
	void OnBackgroundButtonClicked();

	UFUNCTION()
	void OnUseNumChanged(float NewValue);

	UFUNCTION()
	void OnUseButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDesc = nullptr;

	//
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* UseItemBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UseNumText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class USlider* UseNumSlider = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* UseButton = nullptr;

	int32 UseNum = 1;
	int32 SelectedItemKey = -1;
};
