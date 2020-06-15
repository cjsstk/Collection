// Copyright Me. All Rights Reserved.


#include "SkillUpgradePopUp.h"

#include "CMS.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeItem.h"
#include "Widgets/InGameCharacterInfoWidget.h"
#include "Widgets/EquipmentUpgradePopUp.h"

#include "Image.h"
#include "TextBlock.h"
#include "Button.h"
#include "ScrollBox.h"

static TAutoConsoleVariable<int32> CVarSkillUpgradeMoneyMultiplier
(
	TEXT("poke.skillUpgradeMoneyMultiplier"),
	10,
	TEXT("Upgrade money is level * this value")
);

void USkillUpgradePopUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddUniqueDynamic(this, &USkillUpgradePopUp::OnBackgroundClicked);
	}

	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.AddUniqueDynamic(this, &USkillUpgradePopUp::OnUpgradeButtonClicked);
	}
}

void USkillUpgradePopUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TickCheckUpgradeEnable();
}

void USkillUpgradePopUp::InitInfo(int32 InSkillKey, int32 InSkillIndex)
{
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

	UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
	if (!InfoWidget)
	{
		return;
	}

	SelectedCharacter = Player->GetCharacterByID(InfoWidget->GetSelectedCharacterID());
	if (!SelectedCharacter)
	{
		return;
	}

	TArray<int32> SkillLevels;
	SelectedCharacter->GetSkillLevels(SkillLevels);

	const FSkillInfo* SkillInfo = CMS::GetSkillDataTable(InSkillKey);
	if (!ensure(SkillInfo))
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

	SkillIndex = InSkillIndex;
	SkillKey = InSkillKey;

	if (!SkillLevels.IsValidIndex(SkillIndex))
	{
		return;
	}

	if (SkillImage)
	{
		SkillImage->SetBrushFromTexture(SkillInfo->SkillIcon.LoadSynchronous());
	}

	if (SkillName)
	{
		SkillName->SetText(SkillInfo->SkillName);
	}

	int32 SkillLevel = SkillLevels[SkillIndex];

	if (SkillLevelText)
	{
		SkillLevelText->SetText(FText::AsCultureInvariant(FString::FromInt(SkillLevel)));
	}

	NeedMoney = (SkillLevel) * CVarSkillUpgradeMoneyMultiplier.GetValueOnGameThread();

	SetNeedMoneyText(NeedMoney);

	NeedItemBox->ClearChildren();
	NeedItemSlots.Empty();

	TMap<int32, int32> NeedItems;
	CMS::GetSkillUpgradeInfo(SkillLevel, NeedItems);

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

void USkillUpgradePopUp::OnBackgroundClicked()
{
	RemoveFromViewport();
}

void USkillUpgradePopUp::OnUpgradeButtonClicked()
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

	if (!SelectedCharacter)
	{
		return;
	}

	TArray<int32> SkillLevels;
	SelectedCharacter->GetSkillLevels(SkillLevels);

	if (!SkillLevels.IsValidIndex(SkillIndex))
	{
		return;
	}

	int32 SkillLevel = SkillLevels[SkillIndex];
	NeedMoney = (SkillLevel)* CVarSkillUpgradeMoneyMultiplier.GetValueOnGameThread();

	/** Spend Items */
	TMap<int32, int32> NeedItems;
	CMS::GetSkillUpgradeInfo(SkillLevel, NeedItems);

	Player->DeleteItemsByKey(NeedItems);
	Player->SetMoneyAmount(Player->GetMoneyAmount() - NeedMoney);
	//

	int32 NewLevel = SkillLevel + 1;
	SelectedCharacter->SetSkillLevel(SkillIndex, NewLevel);

	InitInfo(SkillKey, SkillIndex);

	UInGameCharacterInfoWidget* InfoWidget = PokeHud->GetInGameCharacterInfoWidget();
	if (InfoWidget && InfoWidget->IsInViewport())
	{
		InfoWidget->OnOpen();
	}
}

void USkillUpgradePopUp::SetNeedMoneyText(int32 InNeedMoney)
{
	if (NeedMoneyText)
	{
		NeedMoneyText->SetText(FText::AsCultureInvariant(FString::FromInt(InNeedMoney)));
	}
}

void USkillUpgradePopUp::TickCheckUpgradeEnable()
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
