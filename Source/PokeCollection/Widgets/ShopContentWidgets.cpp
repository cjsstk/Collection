// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopContentWidgets.h"

#include "ScrollBox.h"

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

void UEquipmentShopWidget::OnOpen()
{
	Super::OnOpen();
}