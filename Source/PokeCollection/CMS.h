// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CMSType.h"
#include "PokeCharacter.h"
#include "PaperFlipbook.h"
#include "CMS.generated.h"

//using characterKey = int32;
//const static characterKey INVALID_CHARACTERKEY = 0;

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* CharacterSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* CharacterProfile;
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
};

namespace CMS
{
	void LoadCMS();

	const FCharacterInfo* GetCharacterDataTable(characterKey CharacterKey);
	const FStageInfo* GetStageDataTable(int32 StageKey);
	void GetAllStageDataTable(TArray<FStageInfo*>& OutArray);
	const FBattleStageInfo* GetBattleStageDataTable(battleStageKey BattleStageKey);

	static UDataTable* CharacterDataTable;
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
