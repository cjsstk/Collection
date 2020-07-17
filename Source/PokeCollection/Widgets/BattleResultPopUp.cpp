// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleResultPopUp.h"

#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "ScrollBox.h"

#include "BattleManager.h"
#include "CMS.h"
#include "PokeCollectionHUD.h"
#include "PokeCore.h"

void UBattleResultPopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UBattleResultPopUp::OnBackgroundButtonClicked);
	}
}

void UBattleResultPopUp::InitResultInfo(FBattleReward& InBattleReward)
{
	BattleReward = InBattleReward;

	if (RewardBox)
	{
		RewardBox->ClearChildren();
	}

	if (!RewardSlotClass.Get())
	{
		return;
	}

	for (int32 CharacterKey : BattleReward.GetCharacters)
	{
		UBattleRewardSlot* RewardSlot = CreateWidget<UBattleRewardSlot>(GetOwningPlayer(), RewardSlotClass.Get());
		if (RewardSlot)
		{
			RewardSlot->InitSlot(ERewardSlotType::Character, CharacterKey, 1);
			RewardBox->AddChild(RewardSlot);
		}
	}

	for (auto&& ItemInfo : BattleReward.GetItems)
	{
		UBattleRewardSlot* RewardSlot = CreateWidget<UBattleRewardSlot>(GetOwningPlayer(), RewardSlotClass.Get());
		if (RewardSlot)
		{
			RewardSlot->InitSlot(ERewardSlotType::Item, ItemInfo.Key, ItemInfo.Value);
			RewardBox->AddChild(RewardSlot);
		}
	}

	if (InBattleReward.MoneyAmount > 0)
	{
		UBattleRewardSlot* RewardSlot = CreateWidget<UBattleRewardSlot>(GetOwningPlayer(), RewardSlotClass.Get());
		if (RewardSlot)
		{
			RewardSlot->InitSlot(ERewardSlotType::Money, 0, InBattleReward.MoneyAmount);
			RewardBox->AddChild(RewardSlot);
		}
	}
}

void UBattleResultPopUp::OnBackgroundButtonClicked()
{
	RemoveFromViewport();

	ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
	if (!ensure(BattleManager))
	{
		return;
	}

	if (BattleManager->IsLastBattle())
	{
		APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
		if (PokeHud)
		{
			PokeHud->OpenInGameMakePartyWidget(true);

			UInGameTopStatusBar* TopStatusBar = PokeHud->GetInGameTopStatusBar();
			if (ensure(TopStatusBar))
			{
				TopStatusBar->AddToViewport(1);
			}
		}

		BattleManager->BattleShutdown();
	}
	else
	{
		BattleManager->StartNextBattle();
	}
}

void UBattleLosePopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UBattleLosePopUp::OnBackgroundButtonClicked);
	}
}

void UBattleLosePopUp::OnBackgroundButtonClicked()
{
	RemoveFromViewport();

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (PokeHud)
	{
		PokeHud->OpenInGameMakePartyWidget(true);

		UInGameTopStatusBar* TopStatusBar = PokeHud->GetInGameTopStatusBar();
		if (ensure(TopStatusBar))
		{
			TopStatusBar->AddToViewport(1);
		}
	}

	ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
	if (ensure(BattleManager))
	{
		BattleManager->BattleShutdown();
	}
}

void UBattleRewardSlot::InitSlot(const ERewardSlotType& InRewardType, int32 InRewardKey, int32 InRewardNum)
{
	if (!RewardImage || !RewardNumText)
	{
		return;
	}

	RewardNumText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	switch (InRewardType)
	{
	case ERewardSlotType::Character:
	{
		const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(InRewardKey);
		if (CharacterInfo)
		{
			RewardImage->SetBrushFromSoftTexture(CharacterInfo->CharacterBattleProfile);
			RewardNumText->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}
	case ERewardSlotType::Item:
	{
		const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(InRewardKey);
		if (ItemInfo)
		{
			RewardImage->SetBrushFromMaterial(PokeCore::GetItemIcon(ItemInfo->ItemIconIndex, this));
			RewardNumText->SetText(FText::AsCultureInvariant(FString::FromInt(InRewardNum)));
		}
		break;
	}
	case ERewardSlotType::Money:
	{
		RewardImage->SetBrushFromMaterial(PokeCore::GetItemIcon(647, this));
		RewardNumText->SetText(FText::AsCultureInvariant(FString::FromInt(InRewardNum)));
		break;
	}
	default:
		break;
	}
}
