// Copyright Me. All Rights Reserved.


#include "CharacterDetailInfoWidget.h"

#include "Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TextBlock.h"

#include "CMS.h"
#include "PokeCharacter.h"

void UCharacterDetailInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCharacterDetailInfoWidget::OnOpen()
{
	Super::OnOpen();

	APokeCharacter* SelectedCharacter = GetSelectedCharacter();
	if (ensure(SelectedCharacter))
	{
		if (CharacterLevel)
		{
			CharacterLevel->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetLevel())));
		}

		if (CurrentExp)
		{
			CurrentExp->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetCurrentExp())));
		}

		if (MaxExp)
		{
			MaxExp->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetMaxExp())));
		}

		FStatus FinalStatus = SelectedCharacter->GetFinalStatus();

		if (HealthPoint)
		{
			HealthPoint->SetText(FText::FromString(FString::FromInt(FinalStatus.HealthPoint)));
		}

		if (MaxHealthPoint)
		{
			MaxHealthPoint->SetText(FText::FromString(FString::FromInt(FinalStatus.HealthPoint)));
		}

		if (Attack)
		{
			Attack->SetText(FText::FromString(FString::FromInt(FinalStatus.Attack)));
		}

		if (Defence)
		{
			Defence->SetText(FText::FromString(FString::FromInt(FinalStatus.Defense)));
		}

		if (SpecialAttack)
		{
			SpecialAttack->SetText(FText::FromString(FString::FromInt(FinalStatus.SpecialAttack)));
		}

		if (SpecialDefence)
		{
			SpecialDefence->SetText(FText::FromString(FString::FromInt(FinalStatus.SpecialDefense)));
		}

		if (Speed)
		{
			Speed->SetText(FText::FromString(FString::FromInt(FinalStatus.Speed)));
		}

		if (AttackRange)
		{
			AttackRange->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetAttackRange())));
		}

		if (ConsumeBerryAmount)
		{
			ConsumeBerryAmount->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetConsumeBerryAmount())));
		}

	}
}