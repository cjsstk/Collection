// Copyright Me. All Rights Reserved.


#include "MakePartySlotsStatusInfo.h"

#include "TextBlock.h"

#include "PokeCollectionCharacter.h"
#include "PokeCharacter.h"

void UMakePartySlotsStatusInfo::Refresh()
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}

	int32 PartyNum = Player->GetCurrentSelectedPartyNum();
	
	const TMap<int32, APokeCharacter*> PartyCharacters = Player->GetPartyCharacters(PartyNum);

	int32 TotalHealth = 0;
	int32 TotalAttack = 0;
	int32 TotalDefense = 0;
	int32 TotalSpAttack = 0;
	int32 TotalSpDefense = 0;
	int32 TotalSpeed = 0;
	int32 TotalConsume = 0;

	for (auto&& PartyCharacter : PartyCharacters)
	{
		APokeCharacter* Character = PartyCharacter.Value;
		if (!Character)
		{
			continue;
		}

		const FStatus& CharacterStatus = Character->GetFinalStatus();

		TotalHealth += CharacterStatus.HealthPoint;
		TotalAttack += CharacterStatus.Attack;
		TotalDefense += CharacterStatus.Defense;
		TotalSpAttack += CharacterStatus.SpecialAttack;
		TotalSpDefense += CharacterStatus.SpecialDefense;
		TotalSpeed += CharacterStatus.Speed;
		TotalConsume += Character->GetConsumeBerryAmount();
	}

	if (TotalHealthText)
	{
		TotalHealthText->SetText(FText::AsNumber(TotalHealth));
	}

	if (TotalAttackText)
	{
		TotalAttackText->SetText(FText::AsNumber(TotalAttack));
	}

	if (TotalDefenseText)
	{
		TotalDefenseText->SetText(FText::AsNumber(TotalDefense));
	}

	if (TotalSpAttackText)
	{
		TotalSpAttackText->SetText(FText::AsNumber(TotalSpAttack));
	}

	if (TotalSpDefenseText)
	{
		TotalSpDefenseText->SetText(FText::AsNumber(TotalSpDefense));
	}

	if (TotalSpeedText)
	{
		TotalSpeedText->SetText(FText::AsNumber(TotalSpeed));
	}

	if (TotalConsumeText)
	{
		TotalConsumeText->SetText(FText::AsNumber(TotalConsume));
	}
}
