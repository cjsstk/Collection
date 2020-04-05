// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMSType.h"
#include "BattleManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleEnd);

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

	void SetBattleStageKey(battleStageKey InBattleStageKey) { CurrentBattleStageKey = InBattleStageKey; };

	class AInBattleCharacterPanel* GetBattlePanel(int32 PanelNum, bool bIsEnemyPanel);

	FOnBattleStart OnBattleStart;
	FOnBattleEnd OnBattleEnd;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
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
};
