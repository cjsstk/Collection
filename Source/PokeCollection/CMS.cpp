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
