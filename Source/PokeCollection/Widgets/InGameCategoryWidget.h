// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "CMSType.h"
#include "InGameCategoryWidget.generated.h"


UCLASS()
class POKECOLLECTION_API UCategoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnButtonClicked();

	void SetSwitcherIndex(int32 InIndex) { SwitcherIndex = InIndex; }
	void SetCategoryName(const FText& InCategoryName);
	void SetCategoryType(ECategoryWidgetType& InCategoryType) { CategoryType = InCategoryType; }

	UFUNCTION()
	void OnCategoryButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CategoryNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* CategoryButton = nullptr;

	ECategoryWidgetType CategoryType = ECategoryWidgetType::Invalid;

	int32 SwitcherIndex = 0;
};

UCLASS()
class POKECOLLECTION_API UCategoryContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnOpen() {}

};

USTRUCT()
struct FCategoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText CategoryName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCategoryContentWidget> ContentWidgetClass = nullptr;
};


/**
 * 카테고리를 가진 위젯 베이스
 */
UCLASS(abstract)
class POKECOLLECTION_API UInGameCategoryWidget : public UInGameWidget
{
	GENERATED_BODY()

public:
	virtual void OnOpen() override;
	virtual void SwitchContentWidget(int32 InContentIndex);

	int32 GetSelectedContentWidgetIndex() const { return SelectedContentWidgetIndex; }

protected:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CatergoryScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ContentsBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	ECategoryWidgetType CategoryType = ECategoryWidgetType::Invalid;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FCategoryStruct> ContentWidgets;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCategoryButtonWidget> CategoryButtonClass;

	UPROPERTY(Transient)
	TArray<class UCategoryContentWidget*> Contents;

	int32 SelectedContentWidgetIndex = 0;
};
