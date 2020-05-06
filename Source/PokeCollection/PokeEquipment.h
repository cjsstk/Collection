// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CMSType.h"
#include "SortObjectInterface.h"
#include "PokeEquipment.generated.h"

USTRUCT(Atomic)
struct FEquipmentStatus
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Attack = 0;

	UPROPERTY()
	int32 Defense = 0;

	UPROPERTY()
	int32 SpecialAttack = 0;

	UPROPERTY()
	int32 SpecialDefense = 0;

	UPROPERTY()
	int32 Speed = 0;

	UPROPERTY()
	int32 AttackRange = 0;
};

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

	void SetOwnerCharacterID(int32 InCharacterID) { OwnerCharacterID = InCharacterID; }
	int32 GetOwnerCharacterID() const { return OwnerCharacterID; }

	int32 GetLevel() const { return Level; }
	const FEquipmentStatus GetFinalEquipmentStatus();
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

	int32 OwnerCharacterID = -1;

	FEquipmentStatus EquipmentStatus;
};
