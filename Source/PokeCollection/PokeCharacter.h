// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "CMSType.h"
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

struct SlotNum
{
	SlotNum(int32 InSlotNum) { SlotNumber = InSlotNum; }

private:
	int32 SlotNumber = 0;

	FORCEINLINE bool operator==(const int32& Other) const
	{
		return SlotNumber == Other;
	}
};

UCLASS()
class POKECOLLECTION_API APokeCharacter : public AActor
{
	GENERATED_BODY()
	
public:
	void Init(characterKey InCharacterKey);

	void Attack(/*int32 SkillIndex,*/ APokeCharacter* TargetCharacter);

	void AddDebugString(const FString& InDebugString, bool bAddNewLine/* = true*/);

	void SetCharacterID(int32 InID) { CharacterID = InID; }
	int32 GetCharacterID() const { return CharacterID; }

	int32 GetCharacterKey() const { return CharacterKey; }

	void SetJoinedPartyNum(int32 InPartyNum) { JoinedPartyNum = InPartyNum; }
	int32 GetJoinedPartyNum() const { return JoinedPartyNum; }

	void SetJoinedSlotNum(int32 InSlotNum) { if (InSlotNum > 9) { ensure(0); return; } JoinedSlotNum = InSlotNum; }
	int32 GetJoinedSlotNum() const { return JoinedSlotNum; }

	int32 GetLevel() const { return Level; }

	FName GetCharacterName() const;
	class UTexture2D* GetCharacterProfileImage() const;
	class UTexture2D* GetCharacterImage() const;
	class UPaperFlipbook* GetCharacterFlipbook() const;
	bool IsRangeAttack() const;

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	int32 CharacterID = 0;
	int32 CharacterKey = 1;
	
	/** 
	 * Base info
	 */
	/*UPROPERTY(Transient)
	FName CharacterName;

	UPROPERTY(Transient)
	class UTexture2D* CharacterProfileImage = nullptr;

	UPROPERTY(Transient)
	class UTexture2D* CharacterImage = nullptr;*/

	/**
	 * Stats
	 */
	UPROPERTY()
	FBaseStats BaseStats;

	int32 Level = 1;

	/** 
	 * In Battle
	 */
	class AInBattleCharacterPanel* CurrentBattlePanel = nullptr;
	bool bAttacking = false;
	
	UPROPERTY(Transient)
	APokeCharacter* CurrentTargetCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABattleCharacterActor> BattleCharacterActor;

	/**
	 * Party
	 */
	int32 JoinedPartyNum = 0;
	int32 JoinedSlotNum = 0;

	FString DebugString;
};
