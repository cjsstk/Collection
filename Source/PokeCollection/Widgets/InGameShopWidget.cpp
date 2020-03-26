// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameShopWidget.h"

#include "TextBlock.h"
#include "ScrollBox.h"
#include "WidgetSwitcher.h"

void UInGameShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ensure(CatergoryMenuScrollBox))
	{
		return;
	}

	CatergoryMenuScrollBox->ClearChildren();

	if (!ensure(ShopContentsBox))
	{
		return;
	}

	ShopContentsBox->ClearChildren();

	for (int32 ContextIndex = 0; ContextIndex < ShopContentWidgets.Num(); ++ContextIndex)
	{
		if (!ShopContentWidgets[ContextIndex].ContentWidgetClass.Get())
		{
			ensure(0);
			return;
		}


		if (!ShopCategoryButtonClass.Get())
		{
			ensure(0);
			break;
		}

		UShopCategoryButtonWidget* ShopCategoryButton = CreateWidget<UShopCategoryButtonWidget>(GetOwningPlayer(), ShopCategoryButtonClass.Get());
		if (ensure(ShopCategoryButton))
		{
			ShopCategoryButton->SetSwitcherIndex(ContextIndex);
			ShopCategoryButton->SetCategoryName(ShopContentWidgets[ContextIndex].CategoryName);
			CatergoryMenuScrollBox->AddChild(ShopCategoryButton);
		}

		UShopContentWidget* ShopContentWidget = CreateWidget<UShopContentWidget>(GetOwningPlayer(), ShopContentWidgets[ContextIndex].ContentWidgetClass.Get());
		if (ensure(ShopContentWidget))
		{
			ShopContentsBox->AddChild(ShopContentWidget);
		}

	}

	ShopContentsBox->SetActiveWidgetIndex(0);
}

void UShopCategoryButtonWidget::SetCategoryName(const FText& InCategoryName)
{
	if (!ensure(CategoryNameText))
	{
		return;
	}

	CategoryNameText->SetText(InCategoryName);
}

void UShopContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnOpen();
}
