// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "PokeCharacter.generated.h"

USTRUCT(Atomic)
struct FBaseStats
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 HealthPoint = 0;

	UPROPERTY()
	int32 Attack = 0;

	UPROPERTY()
	int32 Defense = 0;

	UPROPERTY()
	int32 SpecialAttack = 0;

	UPROPERTY()
	int32 SpecialDefense = 0;

	UPROPERTY()
	int32 Speed = 0;
};


UCLASS()
class POKECOLLECTION_API APokeCharacter : public APaperFlipbookActor
{
	GENERATED_BODY()
	
public:
	APokeCharacter();

	void Attack(/*int32 SkillIndex,*/ APokeCharacter* TargetCharacter);

	void AddDebugString(const FString& InDebugString, bool bAddNewLine/* = true*/);

	void SetCharacterID(int32 InID) { CharacterID = InID; }
	int32 GetCharacterID() const { return CharacterID; }

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	int32 CharacterID = 0;

	/**
	 * Stats
	 */
	UPROPERTY()
	FBaseStats BaseStats;

	/** 
	 * In Battle
	 */
	class AInBattleCharacterPanel* CurrentBattlePanel = nullptr;
	bool bAttacking = false;
	
	UPROPERTY(Transient)
	APokeCharacter* CurrentTargetCharacter = nullptr;

	/**
	 * Party
	 */
	int32 PartyNum = 0;
	int32 SlotNum = 0;

	FString DebugString;
};
