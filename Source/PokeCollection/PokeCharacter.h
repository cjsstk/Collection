// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "CMSType.h"
#include "TypeChart.h"
#include "SortObjectInterface.h"
#include "PokeCharacter.generated.h"

USTRUCT(Atomic)
struct FStatus
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

/**
 * 수집하는 캐릭터 클래스
 */
UCLASS()
class POKECOLLECTION_API APokeCharacter : public AActor, public ISortObjectInterface
{
	GENERATED_BODY()
	
public:
	void Init(FInitCharacterParams& InInitCharacterParams);

	void SetBattleCharacterActor(class ABattleCharacterActor* InBattleCharacterActor);

	/** Use PutOnEquipment of PokeCollectionPlayer instead of this */
	void PutOnEquipment(class UPokeEquipment* InEquipment);
	void TakeOffEquipment();
	class UPokeEquipment* GetCurrentEquipment() const { return CurrentEquipment; }

	void SetCharacterID(int32 InID) { CharacterID = InID; }
	int32 GetCharacterID() const { return CharacterID; }

	int32 GetCharacterKey() const { return CharacterKey; }

	void SetJoinedPartyNum(int32 InPartyNum) { JoinedPartyNum = InPartyNum; }
	int32 GetJoinedPartyNum() const { return JoinedPartyNum; }

	void SetJoinedSlotNum(int32 InSlotNum) { if (InSlotNum > 6) { ensure(0); return; } JoinedSlotNum = InSlotNum; }
	int32 GetJoinedSlotNum() const { return JoinedSlotNum; }

	void SetLevel(int32 NewLevel);
	int32 GetLevel() const { return Level; }

	int32 GetCurrentExp() const { return CurrentExp; }
	int32 GetMaxExp() const { return MaxExp; }

	void TakeExperience(int32 InExp);

	void SetEnemy(bool bInIsEnemy) { bIsEnemy = bInIsEnemy; }
	bool IsEnemy() const { return bIsEnemy; }

	FName GetCharacterName() const;
	class UTexture2D* GetCharacterProfileImage() const;
	class UTexture2D* GetCharacterImage() const;
	class UPaperFlipbook* GetCharacterFlipbook() const;
	ERank GetCharacterRank() const;
	float GetAttackRange(bool bIncludeEquipment = true) const;
	CharacterType GetCharacterType() const { return MyType; }
	int32 GetConsumeBerryAmount() const;

	const FStatus GetFinalStatus(bool bIncludeEquipment = true);

	/** 
	 * ISortObjectInterface Interface
	 */
	virtual int32 GetObjectSortValue(ESortCategory InSortCategory) const override;

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	void InitBaseStatus();
	void InitEvStatus(FStatus InEvStat);
	FStatus CalcFinalStatus(FStatus InBaseStat, FStatus InEvStat);

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
	FStatus BaseStats;

	UPROPERTY()
	FStatus EvStats;

	int32 Level = 1;

	CharacterType MyType;

	int32 CurrentExp = 0;
	int32 MaxExp = 0;

	/** 
	 * In Battle
	 */
	class AInBattleCharacterPanel* CurrentBattlePanel = nullptr;
	bool bIsEnemy = false;

	UPROPERTY(Transient)
	class ABattleCharacterActor* MyBattleCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABattleCharacterActor> BattleCharacterActor;

	/**
	 * Party
	 */
	int32 JoinedPartyNum = 0;
	int32 JoinedSlotNum = 0;

	/** */
	UPROPERTY(Transient)
	class UPokeEquipment* CurrentEquipment = nullptr;
};
