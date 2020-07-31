// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameShopWidget.h"

#include "Button.h"
#include "TextBlock.h"
#include "ScrollBox.h"
#include "WidgetSwitcher.h"
#include "Image.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeCore.h"
#include "Widgets/BuyConfirmPopUp.h"

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

void UShopCategoryButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CategoryButton)
	{
		CategoryButton->OnClicked.AddUniqueDynamic(this, &UShopCategoryButtonWidget::OnCategoryButtonClicked);
	}
}

void UShopCategoryButtonWidget::SetCategoryName(const FText& InCategoryName)
{
	if (!ensure(CategoryNameText))
	{
		return;
	}

	CategoryNameText->SetText(InCategoryName);
}

void UShopCategoryButtonWidget::OnCategoryButtonClicked()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		UInGameShopWidget* ShopWidget = PokeHud->GetInGameShopWidget();
		if (ensure(ShopWidget))
		{
			ShopWidget->SetShopContent(SwitcherIndex);
		}
	}
}

void UShopContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnOpen();
}

void UShopSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectButton)
	{
		SelectButton->SetTouchMethod(EButtonTouchMethod::PreciseTap);
		SelectButton->OnClicked.AddUniqueDynamic(this, &UShopSlot::OnSlotSelected);
	}

	/*APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		UBuyConfirmPopUp* BuyPopUp = PokeHud->GetBuyConfirmPopUp();
		if (ensure(BuyPopUp))
		{
			BuyPopUp->OnBuyButtonClicked.AddUniqueDynamic(this, &UShopSlot::OnBuyThisSlot);
		}
	}*/
}

void UShopSlot::InitSlot(int32 InSlotKey, EShopSlotType InSlotType)
{
	SlotKey = InSlotKey;
	SlotType = InSlotType;

	switch (InSlotType)
	{
	case EShopSlotType::Character:
	{
		const FCharacterShopInfo* ShopInfo = CMS::GetCharacterShopDataTable(SlotKey);
		if (!ShopInfo)
		{
			break;
		}

		SetSlotImage(ShopInfo->EggImage.LoadSynchronous());
		SetSlotName(ShopInfo->EggName);
		SetSlotPrice(ShopInfo->EggPrice);
	}
		break;
	case EShopSlotType::Item:
	{
		const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(SlotKey);
		if (!ItemInfo)
		{
			break;
		}

		SetSlotImageByMaterial(PokeCore::GetItemIcon(ItemInfo->ItemIconIndex, this));
		SetSlotName(ItemInfo->ItemName);
		SetSlotPrice(FText::FromString(FString::FromInt(ItemInfo->ItemPrice)));
	}
		break;
	default:
		break;
	}
}

void UShopSlot::SetSlotImageByMaterial(UMaterialInstanceDynamic* InContentMaterial)
{
	if (!InContentMaterial)
	{
		return;
	}

	SlotImage->SetBrushFromMaterial(InContentMaterial);
}

void UShopSlot::SetSlotImage(UTexture2D* InContentTexture)
{
	if (!InContentTexture)
	{
		return;
	}

	SlotImage->SetBrushFromTexture(InContentTexture);
}

void UShopSlot::SetSlotName(const FText& InName)
{
	SlotNameText->SetText(InName);
}

void UShopSlot::SetSlotPrice(const FText& InPrice)
{
	SlotPriceText->SetText(InPrice);
}

void UShopSlot::OnSlotSelected()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		UBuyConfirmPopUp* BuyPopUp = PokeHud->GetBuyConfirmPopUp();
		if (ensure(BuyPopUp))
		{
			BuyPopUp->OnBuyButtonClicked.AddUniqueDynamic(this, &UShopSlot::OnBuyThisSlot);
		}

		PokeHud->OpenBuyConfirmPopUp(SlotKey, SlotType);
	}
}

void UShopSlot::OnBuyThisSlot(int32 InSlotKey)
{
	switch (SlotType)
	{
	case EShopSlotType::Character:
		OnBuyCharacterSlot(InSlotKey);
		break;
	case EShopSlotType::Item:
		OnBuyItemSlot(InSlotKey);
		break;
	default:
		break;
	}
}

void UShopSlot::OnBuyCharacterSlot(int32 InSlotKey)
{
	if (InSlotKey != SlotKey)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	const FCharacterShopInfo* CurrentShopInfo = CMS::GetCharacterShopDataTable(SlotKey);
	if (!ensure(CurrentShopInfo))
	{
		return;
	}

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!ensure(PokeHud))
	{
		return;
	}

	int32 EggPrice = FCString::Atoi(*(CurrentShopInfo->EggPrice).ToString());

	if (!PaySlotPrice(EggPrice))
	{
		return;
	}

	ERank ComeOutRankResult = ERank::Normal;

	const TMap<ERank, float> ComeOutInfo = CurrentShopInfo->ComeOutCharacterInfo;

	float SSRarePercent = ComeOutInfo.Contains(ERank::SSRare) ? *ComeOutInfo.Find(ERank::SSRare) : 0.0f;
	float SRarePercent = ComeOutInfo.Contains(ERank::SRare) ? *ComeOutInfo.Find(ERank::SRare) : 0.0f;
	float RarePercent = ComeOutInfo.Contains(ERank::Rare) ? *ComeOutInfo.Find(ERank::Rare) : 0.0f;
	float NormalPercent = ComeOutInfo.Contains(ERank::Normal) ? *ComeOutInfo.Find(ERank::Normal) : 0.0f;

	float SSRareRange = SSRarePercent;
	float SRareRange = SSRareRange + SRarePercent;
	float RareRange = SRareRange + RarePercent;

	const int32 RandomInt = FMath::RandRange(0, 10000);

	if (RandomInt <= SSRareRange * 100)
	{
		ComeOutRankResult = ERank::SSRare;
	}
	else if (RandomInt <= SRareRange * 100)
	{
		ComeOutRankResult = ERank::SRare;
	}
	else if (RandomInt <= RareRange * 100)
	{
		ComeOutRankResult = ERank::Rare;
	}

	TArray<FInitCharacterParams> NewCharactersParams;

	TArray<FCharacterInfo*> OutCharacters;
	CMS::GetAllCharacterDataTableByRank(OutCharacters, ComeOutRankResult);

	int32 OutCharacterIndex = FMath::RandRange(0, OutCharacters.Num() - 1);

	if (ensure(OutCharacters.IsValidIndex(OutCharacterIndex) && OutCharacters[OutCharacterIndex]))
	{
		characterKey OutCharacterKey = OutCharacters[OutCharacterIndex]->CharacterKey;

		FInitCharacterParams Params;
		Params.CharacterKey = OutCharacterKey;

		NewCharactersParams.Add(Params);
		//Player->AddNewCharacter(Params);
		PokeHud->OpenEggHatchingWidget(OutCharacterKey);
	}

	Player->AddNewCharacters(NewCharactersParams);

	Player->SaveQuests();
}

void UShopSlot::OnBuyItemSlot(int32 InSlotKey)
{
	if (InSlotKey != SlotKey)
	{
		return;
	}

	const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(InSlotKey);
	if (!ensure(ItemInfo))
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	TArray<FInitItemParams> AddItems;

	FInitItemParams Params;
	Params.ItemKey = InSlotKey;
	Params.ItemStackNum = 1;

	AddItems.Add(Params);

	Player->AddNewItems(AddItems);
}

bool UShopSlot::PaySlotPrice(int32 InEggMoney)
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return false;
	}

	if (InEggMoney > Player->GetMoneyAmount())
	{
		return false;
	}

	Player->SpendMoney(InEggMoney);

	return true;
}

void UInGameShopWidget::SetShopContent(int32 InContentIndex)
{
	ShopContentsBox->SetActiveWidgetIndex(InContentIndex);
}
