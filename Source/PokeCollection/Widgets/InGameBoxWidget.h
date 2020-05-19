// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "CMSType.h"
#include "PokeCore.h"
#include "PokeSortInfo.h"
#include "InGameBoxWidget.generated.h"


UENUM()
enum class EBoxContentType
{
	Character,
	Equipment,
	Item,
};

UCLASS(abstract)
class POKECOLLECTION_API UBoxSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnSelectButtonClicked() {};
	virtual void InitByKey(int32 InContentKey);
	virtual void InitByID(int32 InContentID);

	UFUNCTION()
	void OnSelectContentButtonClicked();

	void SetContentImage(UTexture2D* InContentTexture);
	void SetContentID(int32 InContentID);
	void SetContentName(const FText& InName);
	void SetBackgroundColor(ERank InRank);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* ProfileImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ContentInfoBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectContentButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText = nullptr;

	int32 ContentKey = 0;
	int32 ContentID = 0;

	EBoxContentType BoxContentType = EBoxContentType::Character;
	
};


UCLASS(abstract)
class POKECOLLECTION_API UBoxContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() {};
	virtual void SortContent(FPokeSortInfo InSortInfo);
	virtual void RefreshSlot() {};

	//virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);

protected:
	TArray<class ISortObjectInterface*> SortObject(TArray<class ISortObjectInterface*> InObjects);
	void QuickSort(int32 Left, int32 Right, TArray<class ISortObjectInterface*>& InObjects);

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBoxSlot> BoxSlotClass;

	UPROPERTY(EditDefaultsOnly)
	EBoxContentType BoxContentType = EBoxContentType::Character;

	UPROPERTY(EditDefaultsOnly)
	int32 ColumnNum = 7;

	FPokeSortInfo CurrentSortInfo;
};


USTRUCT()
struct FBoxContentStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	EBoxContentType BoxContentType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBoxContentWidget> ContentWidgetClass = nullptr;
};

UCLASS()
class POKECOLLECTION_API UInGameBoxWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() override;

	UFUNCTION()
	void OnCharacterBoxButtonClicked();
	
	UFUNCTION()
	void OnEquipmentBoxButtonClicked();

	UFUNCTION()
	void SortContentWidget(FPokeSortInfo InSortInfo);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterBoxButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* EquipmentBoxButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ContentsBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class USortWidget* CharacterSortWidget = nullptr;

	UPROPERTY(meta = (BindWidget))
	class USortWidget* EquipmentSortWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FBoxContentStruct> ContentWidgets;

	UPROPERTY(Transient)
	TArray<class UBoxContentWidget*> BoxContents;

	EBoxContentType CurrentBoxContentType = EBoxContentType::Character;
};
