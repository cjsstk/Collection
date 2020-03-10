// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeCore.h"

#include "PokeGameInstance.h"
#include "BattleManager.h"

#include "Engine/World.h"

ABattleManager* PokeCore::GetBattleManager(const UWorld* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}

	UPokeGameInstance* PokeInstance = Cast<UPokeGameInstance>(WorldContext->GetGameInstance());
	if (!ensure(PokeInstance))
	{
		return nullptr;
	}

	return PokeInstance->GetBattleManager();
}
