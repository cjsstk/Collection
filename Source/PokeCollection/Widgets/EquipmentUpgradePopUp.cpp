// Copyright Me. All Rights Reserved.


#include "EquipmentUpgradePopUp.h"

#include "CMS.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeEquipment.h"
#include "PokeItem.h"
#include "Widgets/InGameCharacterInfoWidget.h"

#include "Image.h"
#include "TextBlock.h"
#include "Button.h"
#include "ScrollBox.h"

static TAutoConsoleVariable<int32> CVarEquipmentUpgradeMoneyMultiplier
(
	TEXT("poke.equipmentUpgradeMoneyMultiplier"),
	10,
	TEXT("Upgrade money is level * this value")
);

void UEquipmentUpgradePopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &UEquipmentUpgradePopUp::OnBackgroundClicked);
	}

	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.AddUniqueDynamic(this, &UEquipmentUpgradePopUp::OnUpgradeButtonClicked);
	}
}

void UEquipmentUpgradePopUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TickCheckUpgradeEnable();
}

void UEquipmentUpgradePopUp::InitInfo(int32 InEquipmentID)
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPokeEquipment* Equipment = Player->GetEquipmentByID(InEquipmentID);
	if (!Equipment)
	{
		return;
	}
	
	if (!NeedItemSlotClass.Get())
	{
		return;
	}

	if (!NeedItemBox)
	{
		return;
	}

	EquipmentID = InEquipmentID;

	if (EquipmentImage)
	{
		EquipmentImage->SetBrushFromTexture(Equipment->GetEquipmentProfileImage());
	}

	if (EquipmentName)
	{
		EquipmentName->SetText(FText::FromName(Equipment->GetEquipmentName()));
	}

	int32 EquipmentLevel = Equipment->GetLevel();
	NeedMoney = (EquipmentLevel + 1) * CVarEquipmentUpgradeMoneyMultiplier.GetValueOnGameThread();

	SetNeedMoneyText(NeedMoney);

	NeedItemBox->ClearChildren();
	NeedItemSlots.Empty();

	TMap<int32, int32> NeedItems;
	CMS::GetEquipmentUpgradeInfo(EquipmentLevel, Equipment->GetEquipmentRank(), NeedItems);

	for (auto&& NeedItem : NeedItems)
	{
		UNeedItemSlot* NeedItemSlot = CreateWidget<UNeedItemSlot>(GetWorld(), NeedItemSlotClass.Get());
		if (NeedItemSlot)
		{
			NeedItemBox->AddChild(NeedItemSlot);

			NeedItemSlot->InitByKey(NeedItem.Key);
			NeedItemSlot->SetNeedAmount(NeedItem.Value);
		}

		NeedItemSlots.AddUnique(NeedItemSlot);
	}
}

void UEquipmentUpgradePopUp::OnBackgroundClicked()
{
	RemoveFromViewport();
}

void UEquipmentUpgradePopUp::OnUpgradeButtonClicked()
{
	if (!bUpgradeEnable)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	UPokeEquipment* Equipment = Player->GetEquipmentByID(EquipmentID);
	if (!Equipment)
	{
		return;
	}

	/** Spend Items */
	TMap<int32, int32> NeedItems;
	CMS::GetEquipmentUpgradeInfo(Equipment->GetLevel(), Equipment->GetEquipmentRank(), NeedItems);

	Player->DeleteItemsByKey(NeedItems);
	Player->SetMoneyAmount(Player->GetMoneyAmount() - NeedMoney);
	//

	int32 NewLevel = Equipment->GetLevel() + 1;
	Equipment->SetLevel(NewLevel);

	InitInfo(EquipmentID);

	UInGameBoxWidget* BoxWidget = PokeHud->GetInGameBoxWidget();
	if (BoxWidget && BoxWidget->IsInViewport())
	{
		BoxWidget->OnOpen();
	}

	UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
	if (InfoWidget && InfoWidget->IsInViewport())
	{
		InfoWidget->OnOpen();
	}
}

void UEquipmentUpgradePopUp::SetNeedMoneyText(int32 InNeedMoney)
{
	if (NeedMoneyText)
	{
		NeedMoneyText->SetText(FText::AsCultureInvariant(FString::FromInt(InNeedMoney)));
	}
}

void UEquipmentUpgradePopUp::TickCheckUpgradeEnable()
{
	bUpgradeEnable = false;

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	bUpgradeEnable = (Player->GetMoneyAmount() >= NeedMoney);
	
	for (UNeedItemSlot* NeedItemSlot : NeedItemSlots)
	{
		if (!bUpgradeEnable)
		{
			break;
		}

		bUpgradeEnable = NeedItemSlot->IsEnoughAmount();
	}

	if (UpgradeButton)
	{
		UpgradeButton->SetIsEnabled(bUpgradeEnable);
	}
}

void UNeedItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UNeedItemSlot::InitByKey(int32 InContentKey)
{
	Super::InitByKey(InContentKey);

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	UPokeItem* PokeItem = Player->GetItemByKey(InContentKey);
	if (PokeItem)
	{
		HaveAmount = PokeItem->GetStackNum();
	}
	else
	{
		HaveAmount = 0;
	}
}

void UNeedItemSlot::SetNeedAmount(int32 InNeedAmount)
{
	NeedAmount = InNeedAmount;

	const FString HaveAmountString = FString::FromInt(HaveAmount);
	const FString NeedAmountString = FString::FromInt(NeedAmount);
	FString NeedAmountText = HaveAmountString + TEXT("/") + NeedAmountString;

	SetNeedAmountText(FText::AsCultureInvariant(NeedAmountText));

	bEnoughItem = (HaveAmount >= NeedAmount);
}

void UNeedItemSlot::SetNeedAmountText(const FText& InNeedAmount)
{
	if (NeedText)
	{
		NeedText->SetText(InNeedAmount);
	}
}