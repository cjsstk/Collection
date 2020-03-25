// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InGameMainWidget.h"
#include "Button.h"
#include "InGameShopWidget.generated.h"

UCLASS()
class POKECOLLECTION_API UShopCategoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSwitcherIndex(int32 InIndex) { SwitcherIndex = InIndex; }
	void SetCategoryName(const FText& InCategoryName);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CategoryNameText = nullptr;

	int32 SwitcherIndex = 0;

};


UCLASS(abstract)
class POKECOLLECTION_API UShopSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	//virtual void NativeConstruct() override;

protected:

};

UCLASS(abstract)
class POKECOLLECTION_API UShopContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnOpen() {};

protected:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ShopScrollBox = nullptr;

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
