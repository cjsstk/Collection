// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCore.h"

#include "BattleManager.h"
#include "PokeCollectionGameMode.h"
#include "TypeChart.h"

#include "Engine/World.h"

ABattleManager* PokeCore::GetBattleManager(const UWorld* WorldContext)
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

	return PokeGameMode->GetBattleManager();
}

float PokeCore::GetTypeEffective(EType InAttackType, EType InDefenseType)
{
	int32 EffectiveIndex = ((int32)InAttackType * (int32)EType::Count) + (int32)InDefenseType;
	if (!TypeCharts.IsValidIndex(EffectiveIndex))
	{
		ensure(0);
		return 0;
	}

	return TypeCharts[EffectiveIndex];
}
