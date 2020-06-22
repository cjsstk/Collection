// Fill out your copyright notice in the Description page of Project Settings.


#include "MakePartySlot.h"

#include "Button.h"
#include "TextBlock.h"
#include "VerticalBox.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "Widgets/CharacterBoxWidget.h"
#include "Widgets/InGameMakePartyWidget.h"

void UMakePartySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMakePartySlot::RefreshSlot()
{
	if (!SlotImage)
	{
		return;
	}

	if (!SlotCharacterInfoBox)
	{
		return;
	}

	if (!SlotCharacter)
	{
		return;
	}

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	int32 CurrentPartyNum = Player->GetCurrentSelectedPartyNum();

	const TMap<int32, APokeCharacter*> PartyCharacters = Player->GetPartyCharacters(CurrentPartyNum);

	if (PartyCharacters.Contains(SlotNum))
	{
		APokeCharacter* CurrentSlotCharacter = PartyCharacters[SlotNum];
		if (ensure(CurrentSlotCharacter))
		{
			SlotCharacterInfoBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			SlotCharacter->InitByID(CurrentSlotCharacter->GetCharacterID());

			const FStatus& CharacterStatus = CurrentSlotCharacter->GetFinalStatus();

			SlotCharacterHealth->SetText(FText::AsCultureInvariant(CharacterStatus.HealthPoint));

			bIsEmptySlot = false;
		}
	}
	else
	{
		SlotCharacterInfoBox->SetVisibility(ESlateVisibility::Collapsed);
		bIsEmptySlot = true;
	}
	
}
