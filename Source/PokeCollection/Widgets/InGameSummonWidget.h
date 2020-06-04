// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "InGameSummonWidget.generated.h"

UCLASS()
class POKECOLLECTION_API USummonCategoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetSwitcherIndex(int32 InIndex) { SwitcherIndex = InIndex; }
	void SetCategoryName(const FText& InCategoryName);

	UFUNCTION()
	void OnCategoryButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CategoryNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CategoryButton = nullptr;

	int32 SwitcherIndex = 0;
};


USTRUCT()
struct FSummonCategoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText CategoryName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> ContentWidgetClass = nullptr;
};


/**
 * 소환 화면 위젯
 */
UCLASS()
class POKECOLLECTION_API UInGameSummonWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

	void SwitchContentWidget(int32 InContentIndex);

	int32 GetSelectedContentWidgetIndex() const { return SelectedContentWidgetIndex; }

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CatergoryScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SummonContentsBox = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FSummonCategoryStruct> SummonContentWidgets;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USummonCategoryButtonWidget> SummonCategoryButtonClass;

	UPROPERTY(Transient)
	TArray<class UUserWidget*> SummonContents;

	int32 SelectedContentWidgetIndex = 0;
};

