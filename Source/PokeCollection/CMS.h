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
	UPaperFlipbook* CharacterSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* CharacterIndexProfile;

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

	/** Base Stats */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_HP = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_Attack= 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_Defence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_SPAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_SPDef = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseStat_Speed = 0;
	//


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
	TArray<int32> DropCharacterIDs;

	UPROPERTY(EditDefaultsOnly)
	TArray<FEnemyInfo> FirstEnemyKeys;
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

	const TArray<FCharacterShopInfo*> GetAllCharacterShopData();

	void GetAllCharacterDataTableByRank(TArray<FCharacterInfo*>& OutArray, ERank InRank);
	void GetAllCharacterDataTable(TArray<FCharacterInfo*>& OutArray);

	static UDataTable* CharacterDataTable;
	static UDataTable* EquipmentDataTable;
	static UDataTable* CharacterShopDataTable;
	static UDataTable* StageDataTable;
	static UDataTable* BattleStageDataTable;
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
