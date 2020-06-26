// Copyright Me. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SortObjectInterface.h"
#include "CMSType.h"
#include "PokeItem.generated.h"

/**
 * 기타 아이템 클래스
 */
UCLASS()
class POKECOLLECTION_API UPokeItem : public UObject, public ISortObjectInterface
{
	GENERATED_BODY()
	
public:
	void Init(FInitItemParams& InInitItemParams);

	int32 GetItemID() const { return ItemID; }
	int32 GetItemKey() const { return ItemKey; }
	int32 GetItemIconIndex() const { return ItemIconIndex; }

	const FText& GetItemName() { return ItemName; }
	const FText& GetItemDesc() { return ItemDesc; }
	bool IsUsable() const { return bIsUsable; }
	int32 GetStackNum() const { return StackNum; }
	void SetStackNum(int32 NewStackNum) { StackNum = NewStackNum; }

	// Return reward equipment key
	int32 CalcComeOutEquipmentKey();

	/*FString GetItemName() const;
	class UTexture2D* GetItemImage() const;
	FText GetItemDesc() const;
	bool IsUsable() const;*/

	/**
	 * ISortObjectInterface Interface
	 */
	virtual int32 GetObjectSortValue(ESortCategory InSortCategory) const override;

private:
	int32 ItemID = 0;
	int32 ItemKey = 1;

	ERank ItemRank = ERank::Normal;
	int32 StackNum = 1;

	int32 ItemIconIndex = 0;

	FText ItemName;
	FText ItemDesc;
	bool bIsUsable = false;
};
