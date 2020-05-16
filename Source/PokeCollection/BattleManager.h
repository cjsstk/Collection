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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleLogAdded, FString&, NewBattleLog);

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
	void BattleEnd(bool bIsWin);
	void BattleShutdown();
	void OpenResult(bool bIsWin);

	void AddBattleLog(FString& InBattleLog);

	const TArray<class ABattleCharacterActor*> GetBattleCharacters(bool bIsEnemy);

	int32 GetBattleStageKey() const { return CurrentBattleStageKey; }
	void SetBattleStageKey(battleStageKey InBattleStageKey) { CurrentBattleStageKey = InBattleStageKey; };

	class AInBattleCharacterPanel* GetBattlePanel(int32 PanelNum, bool bIsEnemyPanel);

	float GetTypeEffective(EType InAttackType, EType InDefenseType);

	FOnBattleStart OnBattleStart;
	FOnBattleEnd OnBattleEnd;
	FOnBattleShutdown OnBattleShutdown;
	FOnBattleLogAdded OnBattleLogAdded;

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

	UPROPERTY(Transient)
	TArray<FString> BattleLogs;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABattleCharacterActor> BattleCharacterActorClass;

	battleStageKey CurrentBattleStageKey = INVALID_BATTLESTAGEKEY;

	TArray<float> TypeCharts;

	bool bIsBattlePlaying = false;

	FTimerHandle BattleEndTimerHandle;

	float BattleEndDelaySeconds = 2.0f;
};
