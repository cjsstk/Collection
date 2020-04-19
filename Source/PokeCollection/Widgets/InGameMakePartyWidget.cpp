// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMakePartyWidget.h"

#include "Button.h"
#include "PanelWidget.h"
#include "Kismet/GameplayStatics.h"

#include "BattleManager.h"
#include "PokeCharacter.h"
#include "PokeCore.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "MakePartyCharacterPanel.h"

void UInGameMakePartyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (DecisionButton)
	{
		DecisionButton->OnClicked.AddUniqueDynamic(this, &UInGameMakePartyWidget::OnDecisionButtonClicked);
	}

	CharacterSlots.Empty();

	TArray<AActor*> OutPartySlots;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMakePartyCharacterPanel::StaticClass(), OutPartySlots);

	for (auto&& PartySlot : OutPartySlots)
	{
		if (PartySlot->IsA<AMakePartyCharacterPanel>())
		{
			CharacterSlots.Add(Cast<AMakePartyCharacterPanel>(PartySlot));
		}
	}

}

void UInGameMakePartyWidget::OnOpen()
{
	Super::OnOpen();

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	Player->SetPlayerMode(EPlayerMode::MakePartyMode);
	
	RefreshSlots();
}

void UInGameMakePartyWidget::OnBack()
{
	Super::OnBack();

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	Player->SetPlayerMode(EPlayerMode::UIMode);
}

void UInGameMakePartyWidget::RefreshSlots()
{
	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	const TMap<int32, APokeCharacter*> PartyCharacters = Player->GetPartyCharacters(1);

	for (AMakePartyCharacterPanel* CharacterSlot : CharacterSlots)
	{
		int32 CurrentSlotNum = CharacterSlot->GetSlotNum();
		if (PartyCharacters.Contains(CurrentSlotNum))
		{
			APokeCharacter* CurrentSlotCharacter = PartyCharacters[CurrentSlotNum];
			if (ensure(CurrentSlotCharacter))
			{
				CharacterSlot->SetFlipbook(CurrentSlotCharacter->GetCharacterFlipbook());
				CharacterSlot->SetIsEmptySlot(false);
			}
		}
		else
		{
			CharacterSlot->SetFlipbook(nullptr);
			CharacterSlot->SetIsEmptySlot(true);
		}
	}

}

void UInGameMakePartyWidget::OnDecisionButtonClicked()
{
	if (bJustBeforeBattle)
	{
		ABattleManager* BattleManager = PokeCore::GetBattleManager(GetWorld());
		if (BattleManager)
		{
			BattleManager->BattleStart();
		}
	}
	else
	{
		APokeCollectionHUD* PokeHud = GetPokeHud();
		if (PokeHud)
		{
			PokeHud->OnBackButtonClicked(this);
			OnBack();
		}
	}
}