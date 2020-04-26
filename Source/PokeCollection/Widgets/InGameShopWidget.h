// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "CMSType.h"
#include "InGameShopWidget.generated.h"

UCLASS()
class POKECOLLECTION_API UShopCategoryButtonWidget : public UUserWidget
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


UCLASS(abstract)
class POKECOLLECTION_API UShopSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitSlot(int32 InSlotKey, EShopSlotType InSlotType);

	void SetSlotImage(UTexture2D* InContentTexture);
	void SetSlotName(const FText& InName);
	void SetSlotPrice(const FText& InPrice);

	UFUNCTION()
	void OnSlotSelected();

	UFUNCTION()
	void OnBuyThisSlot(int32 InSlotKey);

private:
	void OnBuyCharacterSlot(int32 InSlotKey);
	void OnBuyItemSlot(int32 InSlotKey);

	bool PayEggPrice(int32 InEggMoney);

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UImage* SlotImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotPriceText = nullptr;

	int32 SlotKey = 0;
	EShopSlotType SlotType = EShopSlotType::Character;
};

UCLASS(abstract)
class POKECOLLECTION_API UShopContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() {};

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopSlot> ShopSlotClass;

};


USTRUCT()
struct FShotCategoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText CategoryName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopContentWidget> ContentWidgetClass = nullptr;
};


UCLASS()
class POKECOLLECTION_API UInGameShopWidget : public UInGameWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetShopContent(int32 InContentIndex);

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CatergoryMenuScrollBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ShopContentsBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopCategoryButtonWidget> ShopCategoryButtonClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FShotCategoryStruct> ShopContentWidgets;

	UPROPERTY(Transient)
	TArray<class UShopContentWidget*> ShopContents;

};
