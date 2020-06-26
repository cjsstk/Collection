// Copyright Me. All Rights Reserved.


#include "UseItemPopUp.h"

#include "Button.h"
#include "HorizontalBox.h"
#include "Image.h"
#include "TextBlock.h"
#include "Slider.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeCore.h"
#include "PokeItem.h"
#include "Widgets/InGameBoxWidget.h"

void UUseItemPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UUseItemPopUp::OnBackgroundButtonClicked);
	}

	if (UseNumSlider)
	{
		UseNumSlider->OnValueChanged.AddUniqueDynamic(this, &UUseItemPopUp::OnUseNumChanged);
	}

	if (UseButton)
	{
		UseButton->OnClicked.AddUniqueDynamic(this, &UUseItemPopUp::OnUseButtonClicked);
	}
}

void UUseItemPopUp::InitInfo(int32 InItemID)
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPokeItem* Item = Player->GetItemByID(InItemID);
	if (!Item)
	{
		return;
	}

	SelectedItemID = InItemID;

	if (ItemImage)
	{
		ItemImage->SetBrushFromMaterial(PokeCore::GetItemIcon(Item->GetItemIconIndex(), this));
	}

	if (ItemName)
	{
		ItemName->SetText(Item->GetItemName());
	}

	if (ItemDesc)
	{
		ItemDesc->SetText(Item->GetItemDesc());
	}

	if (UseNumText)
	{
		FString UseNumString = FString::FromInt(UseNum) + FString(TEXT("개 사용"));
		UseNumText->SetText(FText::AsCultureInvariant(UseNumString));
	}

	if (UseNumSlider)
	{
		UseNumSlider->SetMaxValue(Item->GetStackNum());
		UseNumSlider->SetMinValue(1);

		UseNumSlider->SetValue(UseNum);
	}

	if (UseItemBox)
	{
		UseItemBox->SetVisibility(Item->IsUsable() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UUseItemPopUp::OnBackgroundButtonClicked()
{
	RemoveFromViewport();
}

void UUseItemPopUp::OnUseNumChanged(float NewValue)
{
	UseNum = NewValue;

	if (UseNumText)
	{
		FString UseNumString = FString::FromInt(UseNum) + FString(TEXT("개 사용"));
		UseNumText->SetText(FText::AsCultureInvariant(UseNumString));
	}
}

void UUseItemPopUp::OnUseButtonClicked()
{
	RemoveFromViewport();

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPokeItem* Item = Player->GetItemByID(SelectedItemID);
	if (!Item)
	{
		return;
	}

	TArray<int32> ComeOutEquipmentKeys;
	for (int32 Count = 0; Count < UseNum; ++Count)
	{
		int32 ComeOutEquipKey = Item->CalcComeOutEquipmentKey();
		if (ComeOutEquipKey != INVALID_EQUIPMENTKEY)
		{
			ComeOutEquipmentKeys.Add(ComeOutEquipKey);
		}
	}

	TMap<int32, int32> UseItems;
	UseItems.Add(SelectedItemID, UseNum);

	Player->DeleteItemsByID(UseItems);

	for (int32 EquipKey : ComeOutEquipmentKeys)
	{
		FInitEquipmentParams Params;
		Params.EquipmentKey = EquipKey;

		Player->AddNewEquipment(Params);
	}

	UInGameBoxWidget* BoxWidget = PokeHud->GetInGameBoxWidget();
	if (BoxWidget)
	{
		BoxWidget->OnOpen();
	}
}
