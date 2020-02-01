// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PokeCollectionGameMode.generated.h"

UCLASS(minimalapi)
class APokeCollectionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APokeCollectionGameMode();

	void BattleStart();

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	class ABattleManager* BattleManager = nullptr;

};



