// Copyright Me. All Rights Reserved.


#include "InGameBattleWidget.h"

#include "HorizontalBox.h"
#include "HorizontalBoxSlot.h"
#include "Image.h"
#include "ProgressBar.h"

#include "BattleCharacterActor.h"
#include "BattleCharacterCombatComponent.h"
#include "BattleCharacterHealthComponent.h"
#include "PokeSkill.h"

void UInGameBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillSlotBox->ClearChildren();

	if (!ensure(CharacterSkillSlotClass.Get()))
	{
		return;
	}

	for (int32 SlotNum = 0; SlotNum < 6; ++SlotNum)
	{
		UBattleCharacterSkillSlot* SkillSlot = CreateWidget<UBattleCharacterSkillSlot>(GetWorld(), CharacterSkillSlotClass.Get());
		SkillSlot->SetVisibility(ESlateVisibility::Collapsed);

		/*UHorizontalBoxSlot* BoxSlot = SkillSlotBox->AddChildToHorizontalBox(SkillSlot);
		if (ensure(BoxSlot))
		{
			BoxSlot->SetSize(FSlateChildSize());
			BoxSlot->SetHorizontalAlignment(HAlign_Center);
			BoxSlot->SetVerticalAlignment(VAlign_Center);
		}*/
	}

}

void UInGameBattleWidget::InitBattleCharacters(const TArray<class ABattleCharacterActor*>& InPlayerBattleCharacter)
{
	PlayerBattleCharacters.Empty();

	PlayerBattleCharacters = InPlayerBattleCharacter;

	for (int32 Index = 0; Index < PlayerBattleCharacters.Num(); ++Index)
	{
		if (PlayerBattleCharacters.IsValidIndex(Index))
		{
			UBattleCharacterSkillSlot* SkillSlot = Cast<UBattleCharacterSkillSlot>(SkillSlotBox->GetChildAt(Index));
			if (!ensure(SkillSlot))
			{
				continue;
			}

			SkillSlot->SetOwnerBattleCharacter(*PlayerBattleCharacters[Index]);
			SkillSlot->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UBattleCharacterSkillSlot::SetOwnerBattleCharacter(class ABattleCharacterActor& InBattleCharacter)
{
	OwnerBattleCharacter = &InBattleCharacter;

	InitSlotInfo();
}

void UBattleCharacterSkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ensure(OwnerBattleCharacter))
	{
		return;
	}

	if (HealthBar)
	{
		UBattleCharacterHealthComponent* HealthComp = OwnerBattleCharacter->GetHealthComponent();
		if (HealthComp)
		{
			float HealthPoint = HealthComp->GetHealthPoint();
			float MaxHealthPoint = HealthComp->GetMaxHealthPoint();

			HealthBar->SetPercent(HealthPoint / MaxHealthPoint);
		}
	}

	if (SkillGaugeBar)
	{
		if (!ActiveSkill)
		{
			return;
		}

		float AttackCount = ActiveSkill->GetCurrentAttackCount();
		float MaxAttackCount = ActiveSkill->GetMaxAttackCount();

		SkillGaugeBar->SetPercent(AttackCount / MaxAttackCount);
	}
}

void UBattleCharacterSkillSlot::InitSlotInfo()
{
	if (!ensure(OwnerBattleCharacter))
	{
		return;
	}

	if (CharacterIcon)
	{
		CharacterIcon->SetBrushFromTexture(OwnerBattleCharacter->GetBattleProfile());
	}

	const TArray<class UPokeSkill*>& Skills = OwnerBattleCharacter->GetSkills();

	if (Skills.IsValidIndex(3))
	{
		ensure(0);
		return;
	}

	ActiveSkill = Skills[3];
}
