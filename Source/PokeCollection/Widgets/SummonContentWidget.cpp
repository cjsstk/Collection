// Copyright Me. All Rights Reserved.


#include "SummonContentWidget.h"

#include "Button.h"
#include "TextBlock.h"
#include "ScrollBox.h"

#include "CMS.h"

void USummonContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SummonTypeBox->ClearChildren();

	const TArray<FCharacterShopInfo*> AllShopData = CMS::GetAllCharacterShopData();

	for (int32 ShopDataIndex = 0; ShopDataIndex < AllShopData.Num(); ShopDataIndex++)
	{
		FCharacterShopInfo* ShopData = AllShopData[ShopDataIndex];
		if (!ensure(ShopData))
		{
			continue;
		}

		USummonTypeButton* TypeButton = CreateWidget<USummonTypeButton>(GetOwningPlayer(), SummonTypeButtonClass.Get());
		if (TypeButton)
		{
			TypeButton->SetSummonTypeIndex(ShopDataIndex + 1);
			TypeButton->SetTypeName(ShopData->EggName);
			TypeButton->OnTypeButtonClicked.AddUniqueDynamic(this, &USummonContentWidget::SetSelectedSummonTypeIndex);

			SummonTypeBox->AddChild(TypeButton);
		}
	}

	if (SummonButton)
	{
		SummonButton->OnClicked.AddUniqueDynamic(this, &USummonContentWidget::OnSummonButtonClicked);
	}

	if (SummonPriceText && AllShopData.IsValidIndex(SelectedSummonTypeIndex))
	{
		SummonPriceText->SetText(AllShopData[SelectedSummonTypeIndex]->EggPrice);
	}
}

void USummonContentWidget::SetSelectedSummonTypeIndex(int32 InTypeIndex)
{
	SelectedSummonTypeIndex = InTypeIndex;

	const FCharacterShopInfo* ShopInfo = CMS::GetCharacterShopDataTable(SelectedSummonTypeIndex);
	if (SummonPriceText && ShopInfo)
	{
		SummonPriceText->SetText(ShopInfo->EggPrice);
	}
}

void USummonContentWidget::OnSummonButtonClicked()
{
}

void USummonTypeButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (SummonTypeButton)
	{
		SummonTypeButton->OnClicked.AddUniqueDynamic(this, &USummonTypeButton::OnSummonTypeButtonClicked);
		SummonTypeButton->SetTouchMethod(EButtonTouchMethod::PreciseTap);
	}
}

void USummonTypeButton::SetTypeName(const FText& InTypeName)
{
	if (TypeNameText)
	{
		TypeNameText->SetText(InTypeName);
	}
}

void USummonTypeButton::OnSummonTypeButtonClicked()
{
	OnTypeButtonClicked.Broadcast(SummonTypeIndex);
}

