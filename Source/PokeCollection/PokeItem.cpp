// Copyright Me. All Rights Reserved.


#include "PokeItem.h"

#include "CMS.h"

void UPokeItem::Init(FInitItemParams& InInitItemParams)
{
	if (InInitItemParams.ItemKey == INVALID_ITEMKEY)
	{
		return;
	}

	ItemID = InInitItemParams.ItemID;
	ItemKey = InInitItemParams.ItemKey;
	StackNum = InInitItemParams.ItemStackNum;

	const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(ItemKey);
	if (!ensure(ItemInfo))
	{
		return;
	}

	ItemRank = ItemInfo->ItemRank;
	ItemIconIndex = ItemInfo->ItemIconIndex;
	ItemName = ItemInfo->ItemName;
	ItemDesc = ItemInfo->ItemDesc;
	bIsUsable = ItemInfo->bIsUsable;
}

int32 UPokeItem::CalcComeOutEquipmentKey()
{
	if (!bIsUsable)
	{
		return INVALID_EQUIPMENTKEY;
	}

	const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(ItemKey);
	if (!ensure(ItemInfo))
	{
		return INVALID_EQUIPMENTKEY;
	}

	TMap<int32, float> ComeOutInfo = ItemInfo->ComeOutEquipmentInfo;
	for (auto&& DropEquipment : ComeOutInfo)
	{
		const int32 RandomInt = FMath::RandRange(0, 10000);

		float GetRate = DropEquipment.Value * 100;
		if (GetRate >= RandomInt)
		{
			return DropEquipment.Key;
		}
	}

	return 1;
}

int32 UPokeItem::GetObjectSortValue(ESortCategory InSortCategory) const
{
	int32 SortValue = 0;

	switch (InSortCategory)
	{
	case ESortCategory::Level:
		SortValue = 0;
		break;
	case ESortCategory::Rank:
		SortValue = (int32)ItemRank;
		break;
	case ESortCategory::Index:
		SortValue = ItemKey;
		break;
	case ESortCategory::ID:
		SortValue = ItemID;
		break;
	default:
		break;
	}

	return SortValue;
}
