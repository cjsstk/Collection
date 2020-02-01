// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"

#include "InBattleCharacterPanel.h"
#include "PokeCollectionCharacter.h"

#include "Kismet/GameplayStatics.h"

ABattleManager::ABattleManager()
{
}

void ABattleManager::BattleStart()
{
	if (!ensure(PlayerCharacter))
	{
		return;
	}

	const TArray<APokeCharacter*>& CurrentPartyCharacters = PlayerCharacter->GetFirstPartyCharacters();

	for (auto&& PartyMember : CurrentPartyCharacters)
	{
		BattleMembers.AddUnique(PartyMember);
	}

	

}

AInBattleCharacterPanel* ABattleManager::GetBattlePanel(int32 PanelNum, bool bIsEnemyPanel)
{
	for (auto&& Panel : BattleCharacterPanels)
	{
		if (!Panel)
		{
			return nullptr;
		}

		if (PanelNum == Panel->GetPanelNum() && bIsEnemyPanel == Panel->IsEnemyPanel())
		{
			return Panel;
		}
	}
	return nullptr;
}

void ABattleManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerCharacter = Cast<APokeCollectionCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}

	TArray<AActor*> FoundPanels;
	UGameplayStatics::GetAllActorsOfClass(this, AInBattleCharacterPanel::StaticClass(), FoundPanels);

	for (auto&& PanelActor : FoundPanels)
	{
		AInBattleCharacterPanel* Panel = Cast<AInBattleCharacterPanel>(PanelActor);
		if (ensure(Panel))
		{
			BattleCharacterPanels.Add(Panel);
		}
	}
}

void ABattleManager::Tick(float DeltaTime)
{

}
