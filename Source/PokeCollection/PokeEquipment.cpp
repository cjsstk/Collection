// Fill out your copyright notice in the Description page of Project Settings.


#include "PokeEquipment.h"

#include "CMS.h"

void UPokeEquipment::Init(equipmentKey InEquipmentKey)
{
	if (InEquipmentKey == INVALID_EQUIPMENTKEY)
	{
		return;
	}

	EquipmentKey = InEquipmentKey;
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
