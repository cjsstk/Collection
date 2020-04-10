// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CMSType.h"
#include "SortObjectInterface.h"
#include "PokeEquipment.generated.h"

/**
 * 
 */
UCLASS()
class POKECOLLECTION_API UPokeEquipment : public UObject, public ISortObjectInterface
{
	GENERATED_BODY()
	
public:
	void Init(equipmentKey InEquipmentKey);

	void SetEquipmentID(int32 InID) { EquipmentID = InID; }
	int32 GetEquipmentID() const { return EquipmentID; }

	int32 GetEquipmentKey() const { return EquipmentKey; }

	int32 GetLevel() const { return Level; }

	FName GetEquipmentName() const;
	class UTexture2D* GetEquipmentProfileImage() const;
	FText GetEquipmentDesc() const;

	/**
	 * ISortObjectInterface Interface
	 */
	virtual int32 GetObjectSortValue(ESortCategory InSortCategory) const override;

private:
	int32 EquipmentID = 0;
	int32 EquipmentKey = 1;

	int32 Level = 1;

};
