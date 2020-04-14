// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"

#include "BattleCharacterActor.h"
#include "CMS.h"
#include "InBattleCharacterPanel.h"
#include "PokeCollectionCharacter.h"
#include "PokeCharacter.h"
#include "PokeCore.h"

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
				EnemyCharacter->SetEnemy(true);
				BattleMembers.AddUnique(EnemyCharacter);
			}
		}
	}


	for (APokeCharacter* BattleMember : BattleMembers)
	{
		bool bIsEnemy = BattleMember->IsEnemy();
		AInBattleCharacterPanel* BattlePanel = GetBattlePanel(BattleMember->GetJoinedSlotNum(), bIsEnemy);
		if (ensure(BattlePanel) && BattleCharacterActorClass.Get())
		{
			FHitResult HitResult;
			const FVector EndLocation = BattlePanel->GetActorLocation() + FVector(0, 0, -1000);

			bool bTraced = GetWorld()->LineTraceSingleByChannel(HitResult, BattlePanel->GetActorLocation(), EndLocation, ECollisionChannel::ECC_WorldStatic);
			if (ensure(bTraced))
			{
				FVector SpawnedActorLocation = HitResult.Location;
				FRotator SpawnedActorRotator = FRotator(0, bIsEnemy ? 180 : 0, 0);
				ABattleCharacterActor* BattleCharacter = World->SpawnActor<ABattleCharacterActor>(BattleCharacterActorClass.Get(), SpawnedActorLocation, SpawnedActorRotator, FActorSpawnParameters());

				if (BattleCharacter)
				{
					BattleCharacter->InitBattleCharacter(*BattleMember);
					CreatedBattleCharacters.Add(BattleCharacter);
					BattleMember->SetBattleCharacterActor(BattleCharacter);
				}
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

float ABattleManager::GetTypeEffective(EType InAttackType, EType InDefenseType)
{
	int32 EffectiveIndex = ((int32)InAttackType * (int32)EType::Count) + (int32)InDefenseType;
	if (!TypeCharts.IsValidIndex(EffectiveIndex))
	{
		ensure(0);
		return 0;
	}

	return TypeCharts[EffectiveIndex];
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

	InitTypeEffect();
}

void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattleManager::InitTypeEffect()
{
	TArray<FTypeInfo*> AllTypeInfos;
	CMS::GetAllTypeDataTable(AllTypeInfos);

	int32 TypeNum = (int32)EType::Count;

	TypeCharts.Init(1, TypeNum * TypeNum);

	for (int32 TypeIndex = 0; TypeIndex < AllTypeInfos.Num(); ++TypeIndex)
	{
		FTypeInfo* CurrentTypeInfo = AllTypeInfos[TypeIndex];
		if (!CurrentTypeInfo)
		{
			return;
		}

		EType CurrentType = CurrentTypeInfo->Type;

		for (auto&& AttackEffect : CurrentTypeInfo->TypeAttackEffect)
		{
			EType DefenseType = AttackEffect.Key;
			float CurrentAttackEffect = AttackEffect.Value;

			int32 ChartIndex = ((int32)CurrentType * TypeNum) + (int32)DefenseType;
			TypeCharts[ChartIndex] = CurrentAttackEffect;
		}

	}

}
