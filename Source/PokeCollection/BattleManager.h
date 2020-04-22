// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMSType.h"
#include "TypeChart.h"
#include "BattleManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleShutdown);

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
	void BattleEnd();
	void BattleShutdown();
	void TakeReward();

	void SetBattleStageKey(battleStageKey InBattleStageKey) { CurrentBattleStageKey = InBattleStageKey; };

	class AInBattleCharacterPanel* GetBattlePanel(int32 PanelNum, bool bIsEnemyPanel);

	float GetTypeEffective(EType InAttackType, EType InDefenseType);

	FOnBattleStart OnBattleStart;
	FOnBattleEnd OnBattleEnd;
	FOnBattleShutdown OnBattleShutdown;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void TickCheckBattleEnd();
	void InitTypeEffect();
	void ClearBattleManager();
	void GetBattleReward(battleStageKey InBattleStageKey, FBattleReward& OutBattleReward);

	UPROPERTY(Transient)
	TArray<class APokeCharacter*> BattleMembers;

	UPROPERTY(Transient)
	TArray<class AInBattleCharacterPanel*> BattleCharacterPanels;

	UPROPERTY(Transient)
	TArray<class ABattleCharacterActor*> CreatedBattleCharacters;

	UPROPERTY(Transient)
	class APokeCollectionCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABattleCharacterActor> BattleCharacterActorClass;

	battleStageKey CurrentBattleStageKey = INVALID_BATTLESTAGEKEY;

	TArray<float> TypeCharts;

	bool bIsBattlePlaying = false;

	FTimerHandle BattleEndTimerHandle;

	float BattleEndDelaySeconds = 2.0f;
};
