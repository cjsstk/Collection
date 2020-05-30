// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SummonContentWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTypeButtonClicked, int32, SummonTypeIndex);

UCLASS()
class POKECOLLECTION_API USummonTypeButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetSummonTypeIndex(int32 InIndex) { SummonTypeIndex = InIndex; }
	void SetTypeName(const FText& InTypeName);

	UFUNCTION()
	void OnSummonTypeButtonClicked();

	FOnTypeButtonClicked OnTypeButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TypeNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SummonTypeButton = nullptr;

	int32 SummonTypeIndex = 0;
};

/**
 * 캐릭터 소환 위젯
 */
UCLASS()
class POKECOLLECTION_API USummonContentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetSelectedSummonTypeIndex(int32 InTypeIndex);

	UFUNCTION()
	void OnSummonButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* SummonMainImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SummonTypeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SummonButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SummonPriceText = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USummonTypeButton> SummonTypeButtonClass;

	int32 SelectedSummonTypeIndex = 1;
};
