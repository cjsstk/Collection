// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyConfirmPopUp.h"

#include "Button.h"
#include "TextBlock.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"

#define LOCTEXT_NAMESPACE "BuyConfirm"

void UBuyConfirmPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UBuyConfirmPopUp::OnBackgroundClicked);
	}

	if (BuyButton)
	{
		BuyButton->OnClicked.AddUniqueDynamic(this, &UBuyConfirmPopUp::OnBuyButtonClick);
	}
}

void UBuyConfirmPopUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TickCheckBuyEnable();
}

void UBuyConfirmPopUp::InitText(int32 InSlotKey, EShopSlotType InSlotType)
{
	SelectedSlotKey = InSlotKey;

	FText ConfirmText;

	switch (InSlotType)
	{
	case EShopSlotType::Character:
	{
		const FCharacterShopInfo* ShopInfo = CMS::GetCharacterShopDataTable(InSlotKey);
		if (ensure(ShopInfo))
		{
			SlotName = ShopInfo->EggName;
			SlotPrice = FCString::Atoi(*(ShopInfo->EggPrice).ToString());

			FFormatNamedArguments Arguments;
			Arguments.Add(TEXT("SlotName"), ShopInfo->EggName);
			Arguments.Add(TEXT("SlotPrice"), ShopInfo->EggPrice);

			ConfirmText = FText::Format(NSLOCTEXT("BuyConfirmText", "BuyConfirm", "{SlotName}을 {SlotPrice}원에 부화시키겠습니까?"), Arguments);
		}
	}
		break;
	case EShopSlotType::Item:
		break;
	default:
		break;
	}

	BuyConfirmText->SetText(ConfirmText);
}

void UBuyConfirmPopUp::OnBackgroundClicked()
{
	RemoveFromViewport();
}

void UBuyConfirmPopUp::OnBuyButtonClick()
{
	OnBuyButtonClicked.Broadcast(SelectedSlotKey);

	RemoveFromViewport();
}

void UBuyConfirmPopUp::TickCheckBuyEnable()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	if (!BuyButton || !BuyButtonText)
	{
		ensure(0);
		return;
	}

	if (Player->GetMoneyAmount() >= SlotPrice)
	{
		BuyButton->SetIsEnabled(true);
		BuyButtonText->SetText(FText::FromString(TEXT("확인")));
	}
	else
	{
		BuyButton->SetIsEnabled(false);
		BuyButtonText->SetText(FText::FromString(TEXT("돈이 부족합니다")));
	}
}

#undef LOCTEXT_NAMESPACE