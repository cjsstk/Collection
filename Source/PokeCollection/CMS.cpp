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

	static ConstructorHelpers::FObjectFinder<UDataTable> EquipmentUpgradeDT(TEXT("/Game/CMS/EquipmentUpgradeInfo"));
	if (ensure(EquipmentUpgradeDT.Succeeded()))
	{
		EquipmentUpgradeDataTable = EquipmentUpgradeDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDT(TEXT("/Game/CMS/ItemInfo"));
	if (ensure(ItemDT.Succeeded()))
	{
		ItemDataTable = ItemDT.Object;
	}

    static ConstructorHelpers::FObjectFinder<UDataTable> CharacterShopDT(TEXT("/Game/CMS/CharacterShopInfo"));
    if (ensure(CharacterShopDT.Succeeded()))
    {
        CharacterShopDataTable = CharacterShopDT.Object;
    }

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemShopDT(TEXT("/Game/CMS/ItemShopInfo"));
	if (ensure(ItemShopDT.Succeeded()))
	{
		ItemShopDataTable = ItemShopDT.Object;
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

	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerExperienceDT(TEXT("/Game/CMS/PlayerExperienceTable"));
	if (ensure(PlayerExperienceDT.Succeeded()))
	{
		PlayerExperienceDataTable = PlayerExperienceDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterExperienceDT(TEXT("/Game/CMS/CharacterExperienceTable"));
	if (ensure(CharacterExperienceDT.Succeeded()))
	{
		CharacterExperienceDataTable = CharacterExperienceDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DialogDT(TEXT("/Game/CMS/DialogInfo"));
	if (ensure(DialogDT.Succeeded()))
	{
		DialogDataTable = DialogDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillDT(TEXT("/Game/CMS/SkillInfo"));
	if (ensure(SkillDT.Succeeded()))
	{
		SkillDataTable = SkillDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillUpgradeDT(TEXT("/Game/CMS/SkillUpgradeInfo"));
	if (ensure(SkillUpgradeDT.Succeeded()))
	{
		SkillUpdgradeDataTable = SkillUpgradeDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> BasicPopUpDT(TEXT("/Game/CMS/BasicPopUpInfo"));
	if (ensure(BasicPopUpDT.Succeeded()))
	{
		BasicPopUpDataTable = BasicPopUpDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ItemIconM(TEXT("/Game/UI/Texture/M_ItemIcon_Inst"));
	if (ensure(ItemIconM.Succeeded()))
	{
		ItemIconMaterial = ItemIconM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TypeM(TEXT("/Game/UI/Texture/M_Type_Inst"));
	if (ensure(TypeM.Succeeded()))
	{
		TypeMaterial = TypeM.Object;
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

const FPokeItemInfo* CMS::GetItemDataTable(itemKey ItemKey)
{
	if (ItemKey == INVALID_ITEMKEY)
	{
		return nullptr;
	}

	FPokeItemInfo* ItemInfo = ItemDataTable->FindRow<FPokeItemInfo>(FName(*(FString::FormatAsNumber(ItemKey))), FString(""));

	return ItemInfo;
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

const FItemShopInfo* CMS::GetItemShopDataTable(int32 ItemShopKey)
{
	if (ItemShopKey <= 0)
	{
		return nullptr;
	}

	FItemShopInfo* ItemShopInfo = ItemShopDataTable->FindRow<FItemShopInfo>(FName(*(FString::FormatAsNumber(ItemShopKey))), FString(""));

	return ItemShopInfo;
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

const FPlayerExperienceTable* CMS::GetPlayerExperienceTable(int32 InCurrentLevel)
{
	TArray<FPlayerExperienceTable*> AllTable;
	PlayerExperienceDataTable->GetAllRows(FString(""), AllTable);

	for (FPlayerExperienceTable* ET : AllTable)
	{
		if (ET && ET->Level == InCurrentLevel)
		{
			return ET;
		}
	}

	return nullptr;
}

const FCharacterExperienceTable* CMS::GetCharacterExperienceTable(int32 InCurrentLevel)
{
	TArray<FCharacterExperienceTable*> AllTable;
	CharacterExperienceDataTable->GetAllRows(FString(""), AllTable);

	for (FCharacterExperienceTable* ET : AllTable)
	{
		if (ET && ET->Level == InCurrentLevel)
		{
			return ET;
		}
	}

	return nullptr;
}

const FDialogInfoTable* CMS::GetDialogDataTable(int32 InDialogKey)
{
	if (InDialogKey <= 0)
	{
		return nullptr;
	}

	FDialogInfoTable* DialogInfo = DialogDataTable->FindRow<FDialogInfoTable>(FName(*(FString::FormatAsNumber(InDialogKey))), FString(""));

	return DialogInfo;
}

const FSkillInfo* CMS::GetSkillDataTable(int32 InSkillKey)
{
	if (InSkillKey <= 0)
	{
		return nullptr;
	}

	FSkillInfo* SkillInfo = SkillDataTable->FindRow<FSkillInfo>(FName(*(FString::FormatAsNumber(InSkillKey))), FString(""));

	return SkillInfo;
}

const FBasicPopUpInfo* CMS::GetBasicPopUpDataTable(EBasicPopUpType InPopUpType)
{
	TArray<FBasicPopUpInfo*> AllTable;
	BasicPopUpDataTable->GetAllRows(FString(""), AllTable);

	for (FBasicPopUpInfo* BT : AllTable)
	{
		if (BT && BT->PopUpType == InPopUpType)
		{
			return BT;
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

void CMS::GetSkillUpgradeInfo(int32 InSkillLevel, TMap<int32, int32>& OutItems)
{
	FSkillUpgradeInfo* SkillUpgradeInfo = SkillUpdgradeDataTable->FindRow<FSkillUpgradeInfo>(FName(*(FString::FormatAsNumber(InSkillLevel))), FString(""));

	if (SkillUpgradeInfo)
	{
		OutItems = SkillUpgradeInfo->NeedItems;
	}
}

void CMS::GetEquipmentUpgradeInfo(int32 InEquipmentLevel, ERank InEquipmentRank, TMap<int32, int32>& OutItems)
{
	FEquipmentUpgradeInfo* UpgradeInfo = EquipmentUpgradeDataTable->FindRow<FEquipmentUpgradeInfo>(FName(*(FString::FormatAsNumber(InEquipmentLevel + 1))), FString(""));

	if (UpgradeInfo)
	{
		FUpgradeEquipmentInfo* NeedItems = UpgradeInfo->UpgradeInfos.Find(InEquipmentRank);

		if (NeedItems)
		{
			OutItems = NeedItems->NeedItems;
		}
	}
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

void CMS::GetAllTypeDataTable(TArray<FTypeInfo*>& OutArray)
{
	OutArray.Empty();

	TypeDataTable->GetAllRows(FString(""), OutArray);
}

int32 CMS::GetItemDataNum()
{
	TArray<FPokeItemInfo*> AllItems;
	ItemDataTable->GetAllRows(FString(""), AllItems);

	return AllItems.Num();
}

UMaterialInterface* CMS::GetItemIconMaterial() 
{ 
	return ItemIconMaterial; 
}

UMaterialInterface* CMS::GetTypeMaterial()
{
	return TypeMaterial;
}
