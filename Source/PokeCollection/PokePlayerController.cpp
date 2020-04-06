// Fill out your copyright notice in the Description page of Project Settings.


#include "PokePlayerController.h"

void APokePlayerController::AddBattleLogString(const FString& InBattleLogString)
{
	BattleLogString += InBattleLogString;

	BattleLogString += TEXT("\n");
}

void APokePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
}