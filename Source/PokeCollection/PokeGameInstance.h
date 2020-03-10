// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PokeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UPokeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	class ABattleManager* GetBattleManager() const { return BattleManager; }

protected:
	virtual void Init() override;

private:
	UPROPERTY(Transient)
	class ABattleManager* BattleManager = nullptr;

};
