// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CMSType.generated.h"

using characterKey = int32;
const static characterKey INVALID_CHARACTERKEY = 0;

using equipmentKey = int32;
const static equipmentKey INVALID_EQUIPMENTKEY = 0;

using itemKey = int32;
const static itemKey INVALID_ITEMKEY = 0;

using battleStageKey = int32;
const static battleStageKey INVALID_BATTLESTAGEKEY = 0;

UENUM()
enum class ERank : uint8
{
	Normal,
	Rare,
	SRare,
	SSRare,
};

UENUM()
enum class EStatus : int32
{
	HealthPoint,
	Attack,
	Defense,
	SpAttack,
	SpDefense,
	Speed,
	Count
};

enum class EShopSlotType
{
	Character,
	Item,
};

UENUM()
enum class EBasicPopUpType : int32
{
	Invalid,
	DestroyCharacter,
	DestroyEquipment,
};

UENUM()
enum class EQuestObjectionType : int32
{
	ClearBattleStageNum,
	ClearBattleCount,
	GetCharacterCount,
	SpendBerryAmount,
	SpendMoneyAmount,
};

UENUM()
enum class ECategoryWidgetType
{
	Invalid,
	Quest,
};

enum class ERewardSlotType
{
	Character,
	Item,
	Money,
};

struct FBattleReward
{
public:
	int32 ExperienceAmount = 0;

	int32 MoneyAmount = 0;

	TArray<int32> GetCharacters;
	
	TMap<int32, int32> GetItems;
};

USTRUCT()
struct FDialogTalk
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText TalkPersonName;

	UPROPERTY(EditAnywhere)
	FString TalkContext;
};

USTRUCT()
struct FInitPlayerParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString PlayerNickname;

	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 PlayerCurrentExp = 0;

	UPROPERTY()
	int32 PlayerMoney = 0;

	UPROPERTY()
	int32 PlayerBerry = 0;

	UPROPERTY()
	int32 PlayerStardust = 0;

	UPROPERTY()
	int32 MaxClearBattleStage = 0;

	UPROPERTY()
	int32 LastSelectedChapter = 0;

	UPROPERTY()
	int32 MaxOpenedChapter = 0;

	UPROPERTY()
	int32 LastSelectedParty = 0;

	UPROPERTY()
	int32 BattleClearCount = 0;

	UPROPERTY()
	int32 GetCharacterCount = 0;

	UPROPERTY()
	int32 MaxHaveCharactersNum = 0;

	UPROPERTY()
	int32 MaxHaveEquipmentsNum = 0;

	UPROPERTY()
	FString Index;

	UPROPERTY()
	int32 BattleSpeed = 0;
};

USTRUCT()
struct FInitCharacterParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 CharacterID = -1;

	UPROPERTY()
	int32 CharacterKey = INVALID_CHARACTERKEY;

	UPROPERTY()
	int32 CharacterLevel = 1;

	UPROPERTY()
	int32 CurrentExp = 0;

	UPROPERTY()
	int32 EvHealth = 0;

	UPROPERTY()
	int32 EvAttack = 0;

	UPROPERTY()
	int32 EvDefence = 0;

	UPROPERTY()
	int32 EvSpecialAttack = 0;

	UPROPERTY()
	int32 EvSpecialDefence = 0;

	UPROPERTY()
	int32 EvSpeed = 0;

	UPROPERTY()
	int32 JoinedPartyNum = 0;

	UPROPERTY()
	int32 JoinedSlotNum = 0;

	UPROPERTY()
	int32 Skill1Level = 1;

	UPROPERTY()
	int32 Skill2Level = 1;

	UPROPERTY()
	int32 Skill3Level = 1;

	UPROPERTY()
	int32 Skill4Level = 1;

	UPROPERTY()
	bool bIsEnemy = false;
};

USTRUCT()
struct FInitEquipmentParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 EquipmentID = -1;

	UPROPERTY()
	int32 EquipmentKey = INVALID_EQUIPMENTKEY;

	UPROPERTY()
	int32 EquipmentLevel = 0;

	UPROPERTY()
	int32 OwnerCharacterID = -1;
};

USTRUCT()
struct FInitItemParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 ItemID = -1;

	UPROPERTY()
	int32 ItemKey = INVALID_ITEMKEY;

	UPROPERTY()
	int32 ItemStackNum = 1;

};

struct FUpdateQuestParams
{
	EQuestObjectionType ObjectionType = EQuestObjectionType::ClearBattleCount;

	/** If bIsAdd is true, InCount Added to CurrNum else InCount override CurrNum */
	bool bIsAdd = true;

	int32 InCount = 1;
};

USTRUCT()
struct FUpgradeEquipmentInfo
{
	GENERATED_BODY()

public:
	/** Key: itemKey, Value: itemAmount*/
	UPROPERTY(EditAnywhere)
	TMap<int32, int32> NeedItems;
};

static FLinearColor SSRareColor = FLinearColor(1.0f, 1.0f, 0.2f);
static FLinearColor SRareColor = FLinearColor(0.5f, 0.2f, 1.0f);
static FLinearColor RareColor = FLinearColor(0.2f, 1.0, 1.0f);
static FLinearColor NormalColor = FLinearColor(0.8f, 0.8f, 0.8f);

//static FLinearColor SSRareColor = FLinearColor::FromSRGBColor(FColor(255, 255, 76));
//static FLinearColor SRareColor = FLinearColor::FromSRGBColor(FColor(204, 153, 255));
//static FLinearColor RareColor = FLinearColor::FromSRGBColor(FColor(0, 255, 255));
//static FLinearColor NormalColor = FLinearColor::FromSRGBColor(FColor(204, 204, 204));