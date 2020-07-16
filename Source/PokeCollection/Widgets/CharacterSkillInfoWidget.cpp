// Copyright Me. All Rights Reserved.


#include "CharacterSkillInfoWidget.h"

#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "VerticalBox.h"
#include "Overlay.h"

#include "CMS.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"

extern TAutoConsoleVariable<int32> CVarSkill1ActivateLevel
(
	TEXT("poke.skill1ActivateLevel"),
	5,
	TEXT("Skill1 activated on this level")
);

extern TAutoConsoleVariable<int32> CVarSkill2ActivateLevel
(
	TEXT("poke.skill2ActivateLevel"),
	20,
	TEXT("Skill2 activated on this level")
);

extern TAutoConsoleVariable<int32> CVarSkill3ActivateLevel
(
	TEXT("poke.skill3ActivateLevel"),
	50,
	TEXT("Skill3 activated on this level")
);


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

	int32 CharacterLevel = SelectedCharacter->GetLevel();

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
		Params.SkillLevel = SkillLevels[SkillIndex];

		SkillSlot->InitSkillInfo(Params);

		int32 ActivateLevel = 0;
		switch (SkillIndex)
		{
		case 0:
			ActivateLevel = CVarSkill1ActivateLevel.GetValueOnGameThread();
			break;
		case 1:
			ActivateLevel = CVarSkill2ActivateLevel.GetValueOnGameThread();
			break;
		case 2:
			ActivateLevel = CVarSkill3ActivateLevel.GetValueOnGameThread();
			break;
		default:
			break;
		}

		SkillSlot->SetActivateSlot(CharacterLevel >= ActivateLevel);
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
		UCurveFloat* SkillDamage = SkillInfo->SkillDamageUpgradeCurve.LoadSynchronous();
		if (SkillDamage)
		{
			FString DamageString = TEXT("위력: ") + FString::FromInt(SkillDamage->GetFloatValue(Params.SkillLevel));

			SkillDamageText->SetText(FText::AsCultureInvariant(DamageString));
		}
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

void USkillInfoSlot::SetActivateSlot(bool bActivate)
{
	if (DeactiveMessage)
	{
		DeactiveMessage->SetVisibility(bActivate ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (ActiveLevelMessage)
	{
		int32 ActivateLevel = 0;

		switch (SkillIndex)
		{
		case 0:
			ActivateLevel = CVarSkill1ActivateLevel.GetValueOnGameThread();
			break;
		case 1:
			ActivateLevel = CVarSkill2ActivateLevel.GetValueOnGameThread();
			break;
		case 2:
			ActivateLevel = CVarSkill3ActivateLevel.GetValueOnGameThread();
			break;
		default:
			break;	
		}

		FString ActiveLevelString = FString::FromInt(ActivateLevel) + TEXT("레벨에 활성화");

		ActiveLevelMessage->SetText(FText::AsCultureInvariant(ActiveLevelString));
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
