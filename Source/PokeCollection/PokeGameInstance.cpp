// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeGameInstance.h"

#include "BattleManager.h"

void UPokeGameInstance::Init()
{
	Super::Init();

	BattleManager = NewObject<ABattleManager>();
	if (!BattleManager)
	{
		ensure(0);
	}
}