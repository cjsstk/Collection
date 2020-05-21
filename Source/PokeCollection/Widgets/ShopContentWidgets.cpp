// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopContentWidgets.h"

#include "ScrollBox.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"

#include "CMS.h"
#include "InGameShopWidget.h"

void UCharacterShopWidget::OnOpen()
{
	Super::OnOpen();

	if (!ShopSlotClass.Get())
	{
		return;
	}

	ShopScrollBox->ClearChildren();

	const TArray<FCharacterShopInfo*> AllCharacterShopInfo = CMS::GetAllCharacterShopData();

	for (int32 InfoKey = 0; InfoKey < AllCharacterShopInfo.Num(); ++InfoKey)
	{
		if (!AllCharacterShopInfo[InfoKey])
		{
			return;
		}

		UShopSlot* ShopSlot = CreateWidget<UShopSlot>(GetOwningPlayer(), ShopSlotClass.Get());
		if (!ensure(ShopSlot))
		{
			continue;
		}
		
		ShopSlot->InitSlot(InfoKey + 1, EShopSlotType::Character);
		ShopScrollBox->AddChild(ShopSlot);
	}
}

void UItemShopWidget::OnOpen()
{
	Super::OnOpen();

	if (!ShopSlotClass.Get())
	{
		return;
	}

	ShopGridPanel->ClearChildren();

	const TArray<FCharacterShopInfo*> AllCharacterShopInfo = CMS::GetAllCharacterShopData();

	int32 ItemColumnNum = 5;
	for (int32 InfoKey = 0; InfoKey < ItemColumnNum * 2; ++InfoKey)
	{
		UShopSlot* ShopSlot = CreateWidget<UShopSlot>(GetOwningPlayer(), ShopSlotClass.Get());
		if (!ensure(ShopSlot))
		{
			return;
		}

		const FItemShopInfo* ItemShopInfo = CMS::GetItemShopDataTable(InfoKey + 1);
		if (ItemShopInfo)
		{
			int32 SlotItemKey = ItemShopInfo->ItemKey;

			const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(SlotItemKey);
			if (ItemInfo)
			{
				ShopSlot->InitSlot(SlotItemKey, EShopSlotType::Item);
				ShopSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				ShopSlot->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			ShopSlot->SetVisibility(ESlateVisibility::Hidden);
		}

		UUniformGridSlot* GridSlot = ShopGridPanel->AddChildToUniformGrid(ShopSlot, InfoKey / ItemColumnNum, InfoKey % ItemColumnNum);
		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
		
	}
}