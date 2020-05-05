// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CMSType.h"
#include "PaperFlipbook.h"
#include "TypeChart.h"
#include "CMS.generated.h"

//using characterKey = int32;
//const static characterKey INVALID_CHARACTERKEY = 0;

USTRUCT()
struct FEnemyInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 EnemyCharacterKey;

	UPROPERTY(EditAnywhere)
	int32 EnemyCharacterLevel;

	UPROPERTY(EditAnywhere)
	int32 EnemySlotNum;
};


USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* CharacterSprite_Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* CharacterSprite_Attack;

	/** Slot Image */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterBattleProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* CharacterIndexProfile;

	/** full screen image */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERank CharacterRank = ERank::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EType Type1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EType Type2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConsumeBerryAmount = 5;

	/** Base Stats */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_HP = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_Attack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_Defence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_SPAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_SPDef = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_Speed = 0;
	//

	/** Skill keys. Just until element 3. Last skill is active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SkillKeys;
};

USTRUCT(BlueprintType)
struct FEquipmentInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipmentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EquipmentProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EquipmentDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERank EquipmentRank = ERank::Normal;
};

USTRUCT(BlueprintType)
struct FCharacterShopInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText EggName;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* EggImage;

	UPROPERTY(EditDefaultsOnly)
	FText EggPrice;

	/** Key: characterkey, Value: percent */
	UPROPERTY(EditDefaultsOnly)
	TMap<ERank, float> ComeOutCharacterInfo;
};

USTRUCT(BlueprintType)
struct FStageInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UStageWidget> StageWidgets;

};

USTRUCT(BlueprintType)
struct FBattleStageInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText BattleStageName;

	UPROPERTY(EditDefaultsOnly)
	int32 DropExperience = 0;

	/** Key: characterKey, Value: Rate */
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, float> DropCharacterInfos;

	UPROPERTY(EditDefaultsOnly)
	TArray<FEnemyInfo> FirstEnemyKeys;
};

USTRUCT(BlueprintType)
struct FTypeInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TextureNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EType, float> TypeAttackEffect;
};

USTRUCT(BlueprintType)
struct FPlayerExperienceTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NeedExperienceForNextLevel;
};

USTRUCT(BlueprintType)
struct FCharacterExperienceTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NeedExperienceForNextLevel;
};

USTRUCT(BlueprintType)
struct FDialogInfoTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FDialogTalk> DialogTalk;

};

USTRUCT(BlueprintType)
struct FSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillDamage = 0;

	/** Attack count for using skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASkillProjectileActor> SpawnProjectileActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPokeSkill> SkillClass;

};

namespace CMS
{
	void LoadCMS();

	const FCharacterInfo* GetCharacterDataTable(characterKey CharacterKey);
	const FEquipmentInfo* GetEquipmentDataTable(equipmentKey EquipmentKey);
	const FCharacterShopInfo* GetCharacterShopDataTable(int32 CharShopKey);
	const FStageInfo* GetStageDataTable(int32 StageKey);
	void GetAllStageDataTable(TArray<FStageInfo*>& OutArray);
	const FBattleStageInfo* GetBattleStageDataTable(battleStageKey BattleStageKey);
	const FTypeInfo* GetTypeDataTable(EType InType);
	const FPlayerExperienceTable* GetPlayerExperienceTable(int32 InCurrentLevel);
	const FCharacterExperienceTable* GetCharacterExperienceTable(int32 InCurrentLevel);
	const FDialogInfoTable* GetDialogDataTable(int32 InDialogKey);
	const FSkillInfo* GetSkillDataTable(int32 InSkillKey);

	const TArray<FCharacterShopInfo*> GetAllCharacterShopData();

	void GetAllCharacterDataTableByRank(TArray<FCharacterInfo*>& OutArray, ERank InRank);
	void GetAllCharacterDataTable(TArray<FCharacterInfo*>& OutArray);
	void GetAllTypeDataTable(TArray<FTypeInfo*>& OutArray);

	static UDataTable* CharacterDataTable = nullptr;
	static UDataTable* EquipmentDataTable = nullptr;
	static UDataTable* CharacterShopDataTable = nullptr;
	static UDataTable* StageDataTable = nullptr;
	static UDataTable* BattleStageDataTable = nullptr;
	static UDataTable* TypeDataTable = nullptr;
	static UDataTable* PlayerExperienceDataTable = nullptr;
	static UDataTable* CharacterExperienceDataTable = nullptr;
	static UDataTable* DialogDataTable = nullptr;
	static UDataTable* SkillDataTable = nullptr;
};

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UCMSData : public UDataTable
{
	GENERATED_BODY()
	
public:


};
