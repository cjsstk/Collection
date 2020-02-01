// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:
	ABattleManager();

	void BattleStart();

	class AInBattleCharacterPanel* GetBattlePanel(int32 PanelNum, bool bIsEnemyPanel);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Transient)
	TArray<class APokeCharacter*> BattleMembers;

	UPROPERTY(Transient)
	TArray<class AInBattleCharacterPanel*> BattleCharacterPanels;

	UPROPERTY(Transient)
	class APokeCollectionCharacter* PlayerCharacter;
};
