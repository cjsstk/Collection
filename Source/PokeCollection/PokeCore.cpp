// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCore.h"

#include "BattleManager.h"
#include "Net/HttpActor.h"
#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionGameMode.h"
#include "Level/InGameLevelScriptActor.h"
#include "TypeChart.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

class APokeCollectionCharacter* PokeCore::GetPokePlayer(const UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	return Cast<APokeCollectionCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContext, 0));
}

ABattleManager* PokeCore::GetBattleManager(const UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	AInGameLevelScriptActor* InGameLevel = Cast<AInGameLevelScriptActor>(WorldContext->GetLevelScriptActor());
	if (!InGameLevel)
	{
		return nullptr;
	}

	/*APokeCollectionGameMode* PokeGameMode = Cast<APokeCollectionGameMode>(WorldContext->GetAuthGameMode());
	if (!ensure(PokeGameMode))
	{
		return nullptr;
	}*/

	return InGameLevel->GetBattleManager();
}

AHttpActor* PokeCore::GetHttpActor(const UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	APokeCollectionGameMode* PokeGameMode = Cast<APokeCollectionGameMode>(WorldContext->GetAuthGameMode());
	if (!ensure(PokeGameMode))
	{
		return nullptr;
	}

	return PokeGameMode->GetHttpActor();
}

void PokeCore::AddBattleLog(const UWorld* WorldContext, FString& NewBattleLog)
{
	ABattleManager* BM = GetBattleManager(WorldContext);
	if (!BM)
	{
		return;
	}

	BM->AddBattleLog(NewBattleLog);
}

FString PokeCore::GetStatusString(EStatus InStat)
{
	switch (InStat)
	{
	case EStatus::HealthPoint:
		return FString(TEXT("체력"));
		break;
	case EStatus::Attack:
		return FString(TEXT("공격"));
		break;
	case EStatus::Defense:
		return FString(TEXT("방어"));
		break;
	case EStatus::SpAttack:
		return FString(TEXT("특공"));
		break;
	case EStatus::SpDefense:
		return FString(TEXT("특방"));
		break;
	case EStatus::Speed:
		return FString(TEXT("스피드"));
		break;
	case EStatus::Count:
		break;
	default:
		break;
	}

	return FString();
}

//float PokeCore::GetTypeEffective(EType InAttackType, EType InDefenseType)
//{
//	int32 EffectiveIndex = ((int32)InAttackType * (int32)EType::Count) + (int32)InDefenseType;
//	if (!TypeCharts.IsValidIndex(EffectiveIndex))
//	{
//		ensure(0);
//		return 0;
//	}
//
//	return TypeCharts[EffectiveIndex];
//}
