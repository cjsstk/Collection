// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCore.h"

#include "BattleManager.h"
#include "Net/HttpActor.h"
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
