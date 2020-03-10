// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCore.h"

#include "BattleManager.h"
#include "PokeCollectionGameMode.h"

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
