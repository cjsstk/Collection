// Copyright Me. All Rights Reserved.


#include "CharacterSkillInfoWidget.h"

#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "VerticalBox.h"

#include "CMS.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"

void UCharacterSkillInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCharacterSkillInfoWidget::OnOpen()
{
	Super::OnOpen();

	APokeCharacter* SelectedCharacter = GetSelectedCharacter();
	if (!ensure(SelectedCharacter))
	{
		return;
	}

	if (!SkillSlotBox)
	{
		return;
	}

	if (!SkillInfoSlotClass.Get())
	{
		return;
	}

	SkillSlotBox->ClearChildren();

	TArray<int32> SkillLevels;
	SelectedCharacter->GetSkillLevels(SkillLevels);

	const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(SelectedCharacter->GetCharacterKey());
	if (!ensure(CharacterInfo))
	{
		return;
	}

	const TArray<int32> SkillKeys = CharacterInfo->SkillKeys;
	for (int32 SkillIndex = 0; SkillIndex < 4; ++SkillIndex)
	{
		if (!SkillKeys.IsValidIndex(SkillIndex))
		{
			continue;
		}

		int32 SkillKey = SkillKeys[SkillIndex];

		USkillInfoSlot* SkillSlot = CreateWidget<USkillInfoSlot>(GetWorld(), SkillInfoSlotClass.Get());
		if (!SkillSlot)
		{
			return;
		}

		InitSkillParams Params;
		Params.SkillKey = SkillKey;
		Params.SkillIndex = SkillIndex;

		SkillSlot->InitSkillInfo(Params);
		SkillSlotBox->AddChildToVerticalBox(SkillSlot);
	}

}

void USkillInfoSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkillUpgradeButton)
	{
		SkillUpgradeButton->OnClicked.AddUniqueDynamic(this, &USkillInfoSlot::OnSkillUpgradeButtonClicked);
	}
}

void USkillInfoSlot::InitSkillInfo(const InitSkillParams& Params)
{
	const FSkillInfo* SkillInfo = CMS::GetSkillDataTable(Params.SkillKey);
	if (!ensure(SkillInfo))
	{
		return;
	}

	SkillKey = Params.SkillKey;
	SkillIndex = Params.SkillIndex;

	if (SkillIcon)
	{
		SkillIcon->SetBrushFromTexture(SkillInfo->SkillIcon.LoadSynchronous());
	}

	if (SkillDamageText)
	{
		FString DamageString = TEXT("위력: ") + FString::FromInt(SkillInfo->SkillDamage);

		SkillDamageText->SetText(FText::AsCultureInvariant(DamageString));
	}

	if (SkillNameText)
	{
		SkillNameText->SetText(SkillInfo->SkillName);
	}

	if (SkillDescText)
	{
		SkillDescText->SetText(SkillInfo->SkillDesc);
	}
}

void USkillInfoSlot::OnSkillUpgradeButtonClicked()
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

	PokeHud->OpenSkillUpgradePopUp(SkillKey, SkillIndex);
}
