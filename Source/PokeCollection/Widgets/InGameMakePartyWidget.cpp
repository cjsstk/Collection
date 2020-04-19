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

	if (Party1Button)
	{
		Party1Button->OnClicked.AddUniqueDynamic(this, &UInGameMakePartyWidget::OnParty1ButtonClicked);
	}

	if (Party2Button)
	{
		Party2Button->OnClicked.AddUniqueDynamic(this, &UInGameMakePartyWidget::OnParty2ButtonClicked);
	}

	if (Party3Button)
	{
		Party3Button->OnClicked.AddUniqueDynamic(this, &UInGameMakePartyWidget::OnParty3ButtonClicked);
	}

	if (Party4Button)
	{
		Party4Button->OnClicked.AddUniqueDynamic(this, &UInGameMakePartyWidget::OnParty4ButtonClicked);
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

	int32 CurrentPartyNum = Player->GetCurrentSelectedPartyNum();

	const TMap<int32, APokeCharacter*> PartyCharacters = Player->GetPartyCharacters(CurrentPartyNum);

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

void UInGameMakePartyWidget::OnParty1ButtonClicked()
{
	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	Player->SetCurrentSelectedPartyNum(1);

	RefreshSlots();
}

void UInGameMakePartyWidget::OnParty2ButtonClicked()
{
	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	Player->SetCurrentSelectedPartyNum(2);

	RefreshSlots();
}

void UInGameMakePartyWidget::OnParty3ButtonClicked()
{
	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	Player->SetCurrentSelectedPartyNum(3);

	RefreshSlots();
}

void UInGameMakePartyWidget::OnParty4ButtonClicked()
{
	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	Player->SetCurrentSelectedPartyNum(4);

	RefreshSlots();
}
