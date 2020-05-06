// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeEquipment.h"

#include "CMS.h"
#include "PokeCharacter.h"

void UPokeEquipment::Init(equipmentKey InEquipmentKey)
{
	if (InEquipmentKey == INVALID_EQUIPMENTKEY)
	{
		return;
	}

	EquipmentKey = InEquipmentKey;

	const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(EquipmentKey);
	if (!ensure(EquipmentInfo))
	{
		return;
	}

	EquipmentStatus.Attack = EquipmentInfo->Equipment_Attack;
	EquipmentStatus.Defense = EquipmentInfo->Equipment_Defense;
	EquipmentStatus.SpecialAttack = EquipmentInfo->Equipment_SPAtk;
	EquipmentStatus.SpecialDefense = EquipmentInfo->Equipment_SPDef;
	EquipmentStatus.Speed = EquipmentInfo->Equipment_Speed;
	EquipmentStatus.AttackRange = EquipmentInfo->Equipment_AttackRange;
}

const FEquipmentStatus UPokeEquipment::GetFinalEquipmentStatus()
{
	FEquipmentStatus FinalStatus;

	FinalStatus.Attack = EquipmentStatus.Attack + Level;
	FinalStatus.Defense = EquipmentStatus.Defense + Level;
	FinalStatus.SpecialAttack = EquipmentStatus.SpecialAttack + Level;
	FinalStatus.SpecialDefense = EquipmentStatus.SpecialDefense + Level;
	FinalStatus.Speed = EquipmentStatus.Speed + Level;
	FinalStatus.AttackRange = EquipmentStatus.AttackRange + Level;

	return FinalStatus;
}

FName UPokeEquipment::GetEquipmentName() const
{
	if (EquipmentKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return FName("InvalidKey");
	}

	const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(EquipmentKey);
	if (!ensure(EquipmentInfo))
	{
		return FName("InvalidCMS");
	}

	return EquipmentInfo->EquipmentName;
}

UTexture2D* UPokeEquipment::GetEquipmentProfileImage() const
{
	if (EquipmentKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return nullptr;
	}

	const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(EquipmentKey);
	if (!ensure(EquipmentInfo))
	{
		return nullptr;
	}

	return EquipmentInfo->EquipmentProfile;
}

FText UPokeEquipment::GetEquipmentDesc() const
{
	if (EquipmentKey == INVALID_CHARACTERKEY)
	{
		ensure(0);
		return FText::FromString("InvalidKey");
	}

	const FEquipmentInfo* EquipmentInfo = CMS::GetEquipmentDataTable(EquipmentKey);
	if (!ensure(EquipmentInfo))
	{
		return FText::FromString("InvalidCMS");
	}

	return EquipmentInfo->EquipmentDesc;
}

int32 UPokeEquipment::GetObjectSortValue(ESortCategory InSortCategory) const
{
	int32 SortValue = 0;

	switch (InSortCategory)
	{
	case ESortCategory::Level:
		SortValue = Level;
		break;
	case ESortCategory::Rank:
		SortValue = 0;
		break;
	case ESortCategory::Index:
		SortValue = EquipmentKey;
		break;
	case ESortCategory::ID:
		SortValue = EquipmentID;
		break;
	default:
		break;
	}

	return SortValue;
}
