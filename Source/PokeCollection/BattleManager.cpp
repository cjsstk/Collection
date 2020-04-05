// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"

#include "BattleCharacterActor.h"
#include "CMS.h"
#include "InBattleCharacterPanel.h"
#include "PokeCollectionCharacter.h"
#include "PokeCharacter.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"

ABattleManager::ABattleManager()
{
}

void ABattleManager::BattleStart()
{
	if (!ensure(PlayerCharacter))
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!ensure(World))
	{
		return;
	}

	PlayerCharacter->SetPlayerMode(EPlayerMode::BattleMode);

	/** Set my characters */
	const TMap<int32, APokeCharacter*>& CurrentPartyCharacters = PlayerCharacter->GetPartyCharacters(1);

	for (auto&& PartyMember : CurrentPartyCharacters)
	{
		BattleMembers.AddUnique(PartyMember.Value);
	}

	/** Set enemy characters */
	const FBattleStageInfo* BattleStageInfo = CMS::GetBattleStageDataTable(CurrentBattleStageKey);
	if (ensure(BattleStageInfo))
	{
		TArray<FEnemyInfo> EnemyInfos = BattleStageInfo->FirstEnemyKeys;

		for (auto&& EnemyInfo : EnemyInfos)
		{
			APokeCharacter* EnemyCharacter = NewObject<APokeCharacter>();
			if (ensure(EnemyCharacter))
			{
				EnemyCharacter->Init(EnemyInfo.EnemyCharacterKey);
				EnemyCharacter->SetLevel(EnemyInfo.EnemyCharacterLevel);
				EnemyCharacter->SetJoinedSlotNum(EnemyInfo.EnemySlotNum);
				EnemyCharacter->SetIsEnemy(true);
				BattleMembers.AddUnique(EnemyCharacter);
			}
		}
	}


	for (APokeCharacter* BattleMember : BattleMembers)
	{
		const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(BattleMember->GetCharacterKey());
		AInBattleCharacterPanel* BattlePanel = GetBattlePanel(BattleMember->GetJoinedSlotNum(), BattleMember->GetIsEnemy());
		if (ensure(BattlePanel) && ensure(CharacterInfo) && BattleCharacterActorClass.Get())
		{
			ABattleCharacterActor* BattleCharacter = World->SpawnActor<ABattleCharacterActor>(BattleCharacterActorClass.Get(), BattlePanel->GetActorLocation(), FRotator::ZeroRotator, FActorSpawnParameters());

			if (BattleCharacter)
			{
				BattleCharacter->GetRenderComponent()->SetFlipbook(CharacterInfo->CharacterSprite);
				CreatedBattleCharacters.Add(BattleCharacter);
				BattleMember->SetBattleCharacterActor(BattleCharacter);
			}
		}
	}

	OnBattleStart.Broadcast();
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
		if (!PlayerCharacter)
		{
			ensure(0);
		}
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
