// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"

#include "BattleCharacterActor.h"
#include "CMS.h"
#include "InBattleCharacterPanel.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeCharacter.h"
#include "PokeCore.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

ABattleManager::ABattleManager()
{
	PrimaryActorTick.bCanEverTick = true;
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

	CurrentBattlePhase = EBattlePhase::One;

	PlayerCharacter->SetPlayerMode(EPlayerMode::BattleMode);

	int32 SumConsumeBerryAmount = 0;

	/** Set my characters */
	int32 CurrentPartyNum = PlayerCharacter->GetCurrentSelectedPartyNum();
	const TMap<int32, APokeCharacter*>& CurrentPartyCharacters = PlayerCharacter->GetPartyCharacters(CurrentPartyNum);

	for (auto&& PartyMember : CurrentPartyCharacters)
	{
		if (!ensure(PartyMember.Value))
		{
			continue;
		}

		BattleMembers.AddUnique(PartyMember.Value);

		SumConsumeBerryAmount += PartyMember.Value->GetConsumeBerryAmount();
	}

	PlayerCharacter->ConsumeBerry(SumConsumeBerryAmount);

	/** Set enemy characters */
	const FBattleStageInfo* BattleStageInfo = CMS::GetBattleStageDataTable(CurrentBattleStageKey);
	if (ensure(BattleStageInfo))
	{
		if (BattleStageInfo->EnemyKeys.Contains((int32)CurrentBattlePhase))
		{
			TArray<FEnemyInfo> EnemyInfos = BattleStageInfo->EnemyKeys[(int32)CurrentBattlePhase].EnemyInfos;

			for (auto&& EnemyInfo : EnemyInfos)
			{
				APokeCharacter* EnemyCharacter = NewObject<APokeCharacter>();
				if (ensure(EnemyCharacter))
				{
					FInitCharacterParams Params;
					Params.CharacterKey = EnemyInfo.EnemyCharacterKey;
					Params.CharacterLevel = EnemyInfo.EnemyCharacterLevel;
					Params.JoinedSlotNum = EnemyInfo.EnemySlotNum;
					Params.bIsEnemy = true;

					EnemyCharacter->Init(Params);
					BattleMembers.AddUnique(EnemyCharacter);
				}
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
				FVector SpawnedActorLocation = HitResult.Location - FVector(BattleMember->GetJoinedSlotNum(), 0, 0);
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

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(PC->GetHUD());
		if (PokeHud)
		{

			PokeHud->OpenInGameBattleWidget(GetBattleCharacters(false));
		}
	}

	bIsBattlePlaying = true;
	OnBattleStart.Broadcast();
}

void ABattleManager::BattleEnd(bool bIsWin)
{
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ABattleManager::OpenResult, bIsWin);

	GetWorldTimerManager().SetTimer(BattleEndTimerHandle, RespawnDelegate, BattleEndDelaySeconds, false);

	bIsBattlePlaying = false;
	OnBattleEnd.Broadcast();
}

void ABattleManager::BattleShutdown()
{
	ClearBattleManager();

	OnBattleShutdown.Broadcast();
}

void ABattleManager::StartNextBattle()
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

	CurrentBattlePhase = (EBattlePhase)((int32)CurrentBattlePhase + 1);

	BattleMembers.RemoveAll([](APokeCharacter* PC)
		{
			if (!PC)
			{
				return true;
			}

			if (PC && PC->IsEnemy())
			{
				PC->Destroy();
				return true;
			}

			return false;
		});

	CreatedBattleCharacters.RemoveAll([](ABattleCharacterActor* BC)
		{
			if (!BC)
			{
				return true;
			}

			if (BC && (BC->IsEnemy() || BC->IsDead()))
			{
				BC->Destroy();
				return true;
			}

			return false;
		});

	/** Set enemy characters */
	const FBattleStageInfo* BattleStageInfo = CMS::GetBattleStageDataTable(CurrentBattleStageKey);
	if (ensure(BattleStageInfo))
	{
		if (BattleStageInfo->EnemyKeys.Contains((int32)CurrentBattlePhase))
		{
			TArray<FEnemyInfo> EnemyInfos = BattleStageInfo->EnemyKeys[(int32)CurrentBattlePhase].EnemyInfos;

			for (auto&& EnemyInfo : EnemyInfos)
			{
				APokeCharacter* EnemyCharacter = NewObject<APokeCharacter>();
				if (ensure(EnemyCharacter))
				{
					FInitCharacterParams Params;
					Params.CharacterKey = EnemyInfo.EnemyCharacterKey;
					Params.CharacterLevel = EnemyInfo.EnemyCharacterLevel;
					Params.JoinedSlotNum = EnemyInfo.EnemySlotNum;
					Params.bIsEnemy = true;

					EnemyCharacter->Init(Params);
					BattleMembers.AddUnique(EnemyCharacter);

					AInBattleCharacterPanel* BattlePanel = GetBattlePanel(EnemyCharacter->GetJoinedSlotNum(), true);
					if (ensure(BattlePanel) && BattleCharacterActorClass.Get())
					{
						FHitResult HitResult;
						const FVector EndLocation = BattlePanel->GetActorLocation() + FVector(0, 0, -1000);

						bool bTraced = GetWorld()->LineTraceSingleByChannel(HitResult, BattlePanel->GetActorLocation(), EndLocation, ECollisionChannel::ECC_WorldStatic);
						if (ensure(bTraced))
						{
							FVector SpawnedActorLocation = HitResult.Location - FVector(EnemyCharacter->GetJoinedSlotNum(), 0, 0);
							FRotator SpawnedActorRotator = FRotator(0, 180, 0);
							ABattleCharacterActor* BattleCharacter = World->SpawnActor<ABattleCharacterActor>(BattleCharacterActorClass.Get(), SpawnedActorLocation, SpawnedActorRotator, FActorSpawnParameters());

							if (BattleCharacter)
							{
								BattleCharacter->InitBattleCharacter(*EnemyCharacter);
								CreatedBattleCharacters.Add(BattleCharacter);
								EnemyCharacter->SetBattleCharacterActor(BattleCharacter);
							}
						}
					}
				}
			}
		}
	}

	for (ABattleCharacterActor* BattleMember : CreatedBattleCharacters)
	{
		bool bIsEnemy = BattleMember->IsEnemy();
		if (bIsEnemy)
		{
			continue;	
		}

		int32 SlotNum = BattleMember->GetJoinedSlotNum();

		AInBattleCharacterPanel* BattlePanel = GetBattlePanel(SlotNum, false);
		if (ensure(BattlePanel))
		{
			FHitResult HitResult;
			const FVector EndLocation = BattlePanel->GetActorLocation() + FVector(0, 0, -1000);

			bool bTraced = GetWorld()->LineTraceSingleByChannel(HitResult, BattlePanel->GetActorLocation(), EndLocation, ECollisionChannel::ECC_WorldStatic);
			if (ensure(bTraced))
			{
				FVector SpawnedActorLocation = HitResult.Location - FVector(SlotNum, 0, 0);
				BattleMember->SetActorLocation(SpawnedActorLocation);
			}
		}
	}

	bIsBattlePlaying = true;
	OnBattleStart.Broadcast();
}

void ABattleManager::OpenResult(bool bIsWin)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APokeCollectionHUD* PokeHud = PC ? Cast<APokeCollectionHUD>(PC->GetHUD()) : nullptr;
	if (!PokeHud)
	{
		ensure(0);
		return;
	}

	if (bIsWin)
	{
		FBattleReward Reward;

		if (IsLastBattle())
		{	
			GetBattleReward(CurrentBattleStageKey, Reward);

			if (ensure(PlayerCharacter))
			{
				PlayerCharacter->GetReward(Reward);
				PlayerCharacter->SetMaxClearBattleStage(CurrentBattleStageKey);
			}
		}

		PokeHud->OpenBattleResultPopUp(Reward);
	}
	else
	{
		PokeHud->OpenBattleLosePopUp();
	}
}

void ABattleManager::AddBattleLog(FString& InBattleLog)
{
	BattleLogs.Add(InBattleLog);

	OnBattleLogAdded.Broadcast(InBattleLog);
}

const TArray<class ABattleCharacterActor*> ABattleManager::GetBattleCharacters(bool bIsEnemy)
{
	TArray<class ABattleCharacterActor*> BattleCharacters;

	for (ABattleCharacterActor* BattleCharacter : CreatedBattleCharacters)
	{
		if (!ensure(BattleCharacter))
		{
			return BattleCharacters;
		}

		if (bIsEnemy == BattleCharacter->IsEnemy())
		{
			BattleCharacters.Add(BattleCharacter);
		}
	}

	return BattleCharacters;
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

	TickCheckBattleEnd();
}

void ABattleManager::TickCheckBattleEnd()
{
	if (!bIsBattlePlaying)
	{
		return;
	}

	int32 LeaveMyCharacterNum = 0;
	int32 LeaveEnemyCharacterNum = 0;

	for (auto&& BattleCharacter : CreatedBattleCharacters)
	{
		if (!BattleCharacter)
		{
			continue;
		}

		if (!BattleCharacter->IsDead())
		{
			if (BattleCharacter->IsEnemy())
			{
				LeaveEnemyCharacterNum += 1;
			}
			else
			{
				LeaveMyCharacterNum += 1;
			}
		}
	}

	if (LeaveMyCharacterNum <= 0 || LeaveEnemyCharacterNum <= 0)
	{
		BattleEnd(LeaveEnemyCharacterNum <= 0);
	}
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

void ABattleManager::ClearBattleManager()
{
	BattleMembers.Empty();

	for (auto&& CreatedCharacter : CreatedBattleCharacters)
	{
		if (CreatedCharacter)
		{
			CreatedCharacter->Destroy();
		}
	}

	CreatedBattleCharacters.Empty();
	BattleLogs.Empty();

	GetWorldTimerManager().ClearTimer(BattleEndTimerHandle);
}

void ABattleManager::GetBattleReward(battleStageKey InBattleStageKey, FBattleReward& OutBattleReward)
{
	OutBattleReward.GetCharacters.Empty();

	const FBattleStageInfo* BattleStageInfo = CMS::GetBattleStageDataTable(InBattleStageKey);
	if (!ensure(BattleStageInfo))
	{
		return;
	}

	const TMap<int32, float> DropCharacters = BattleStageInfo->DropCharacterInfos;
	for (auto&& DropCharacter : DropCharacters)
	{
		const int32 RandomInt = FMath::RandRange(0, 10000);

		float GetRate = DropCharacter.Value * 100;
		if (GetRate >= RandomInt)
		{
			OutBattleReward.GetCharacters.Add(DropCharacter.Key);
		}
	}

	const TMap<int32, float> DropItems = BattleStageInfo->DropItemInfos;
	for (auto&& DropItem : DropItems)
	{
		const int32 RandomInt = FMath::RandRange(0, 10000);

		float GetRate = DropItem.Value * 100;
		if (GetRate >= RandomInt)
		{
			const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(DropItem.Key);
			if (ItemInfo)
			{
				OutBattleReward.GetItems.Add(DropItem.Key, FMath::RandRange(1, ItemInfo->GetMaxNum));
			}
		}
	}

	OutBattleReward.ExperienceAmount = BattleStageInfo->DropExperience;
	OutBattleReward.MoneyAmount = BattleStageInfo->DropMoney;
}
