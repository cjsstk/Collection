// Fill out your copyright notice in the Description page of Project Settings.


#include "CMS.h"
#include "UObject/ConstructorHelpers.h"

#include "PokeCharacter.h"

void CMS::LoadCMS()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> CharacterDT(TEXT("/Game/CMS/CharacterInfo"));
    if (ensure(CharacterDT.Succeeded()))
    {
        CharacterDataTable = CharacterDT.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> EquipmentDT(TEXT("/Game/CMS/EquipmentInfo"));
    if (ensure(EquipmentDT.Succeeded()))
    {
        EquipmentDataTable = EquipmentDT.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> CharacterShopDT(TEXT("/Game/CMS/CharacterShopInfo"));
    if (ensure(CharacterShopDT.Succeeded()))
    {
        CharacterShopDataTable = CharacterShopDT.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> StageDT(TEXT("/Game/CMS/StageInfo"));
    if (ensure(StageDT.Succeeded()))
    {
        StageDataTable = StageDT.Object;
    }

	static ConstructorHelpers::FObjectFinder<UDataTable> BattleStageDT(TEXT("/Game/CMS/BattleStageInfo"));
	if (ensure(BattleStageDT.Succeeded()))
	{
		BattleStageDataTable = BattleStageDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> TypeDT(TEXT("/Game/CMS/TypeInfo"));
	if (ensure(TypeDT.Succeeded()))
	{
		TypeDataTable = TypeDT.Object;
	}
}

const FCharacterInfo* CMS::GetCharacterDataTable(characterKey CharacterKey)
{
    if (CharacterKey == INVALID_CHARACTERKEY)
    {
        return nullptr;
    }

    FCharacterInfo* CharacterInfo = CharacterDataTable->FindRow<FCharacterInfo>(FName(*(FString::FormatAsNumber(CharacterKey))), FString(""));

    return CharacterInfo;
}

const FEquipmentInfo* CMS::GetEquipmentDataTable(equipmentKey EquipmentKey)
{
    if (EquipmentKey == INVALID_EQUIPMENTKEY)
    {
        return nullptr;
    }

    FEquipmentInfo* EquipmentInfo = EquipmentDataTable->FindRow<FEquipmentInfo>(FName(*(FString::FormatAsNumber(EquipmentKey))), FString(""));

    return EquipmentInfo;
}

const FCharacterShopInfo* CMS::GetCharacterShopDataTable(int32 CharShopKey)
{
    if (CharShopKey <= 0)
    {
        return nullptr;
    }

    FCharacterShopInfo* CharacterShopInfo = CharacterShopDataTable->FindRow<FCharacterShopInfo>(FName(*(FString::FormatAsNumber(CharShopKey))), FString(""));

    return CharacterShopInfo;
}

const FStageInfo* CMS::GetStageDataTable(int32 StageKey)
{
    if (StageKey <= 0)
    {
        return nullptr;
    }

    FStageInfo* StageInfo = StageDataTable->FindRow<FStageInfo>(FName(*(FString::FormatAsNumber(StageKey))), FString(""));
 
    return StageInfo;
}

void CMS::GetAllStageDataTable(TArray<FStageInfo*>& OutArray)
{
    StageDataTable->GetAllRows(FString(""), OutArray);
}

const FBattleStageInfo* CMS::GetBattleStageDataTable(battleStageKey BattleStageKey)
{
	if (BattleStageKey == INVALID_BATTLESTAGEKEY)
	{
		return nullptr;
	}

	FBattleStageInfo* BattleStageInfo = BattleStageDataTable->FindRow<FBattleStageInfo>(FName(*(FString::FormatAsNumber(BattleStageKey))), FString(""));

	return BattleStageInfo;
}

const FTypeInfo* CMS::GetTypeDataTable(EType InType)
{
	TArray<FTypeInfo*> AllTypes;
	TypeDataTable->GetAllRows(FString(""), AllTypes);

	for (FTypeInfo* Type : AllTypes)
	{
		if (Type && Type->Type == InType)
		{
			return Type;
		}
	}
	
	return nullptr;
}

const TArray<FCharacterShopInfo*> CMS::GetAllCharacterShopData()
{
    TArray<FCharacterShopInfo*> OutInfos;

    CharacterShopDataTable->GetAllRows(FString(""), OutInfos);
 
    return OutInfos;
}

void CMS::GetAllCharacterDataTableByRank(TArray<FCharacterInfo*>& OutArray, ERank InRank)
{
    OutArray.Empty();

    TArray<FCharacterInfo*> AllCharacter;
    CharacterDataTable->GetAllRows(FString(""), AllCharacter);
 
    for (FCharacterInfo* Character : AllCharacter)
    {
        if (Character && Character->CharacterRank == InRank)
        {
            OutArray.Add(Character);
        }
    }
}

void CMS::GetAllCharacterDataTable(TArray<FCharacterInfo*>& OutArray)
{
	OutArray.Empty();

	CharacterDataTable->GetAllRows(FString(""), OutArray);
}
