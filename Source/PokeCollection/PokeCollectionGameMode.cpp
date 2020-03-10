// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PokeCollectionGameMode.h"

#include "BattleManager.h"
#include "PokeCollectionCharacter.h"
#include "CMS.h"

#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

APokeCollectionGameMode::APokeCollectionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	CMS::LoadCMS();
}

void APokeCollectionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void APokeCollectionGameMode::BeginPlay()
{
	Super::BeginPlay();

	BattleManager = Cast<ABattleManager>(UGameplayStatics::GetActorOfClass(this, ABattleManager::StaticClass()));
	if (!BattleManager)
	{
		ensure(0);
	}
}