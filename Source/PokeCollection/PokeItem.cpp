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

	const FPokeItemInfo* ItemInfo = CMS::GetItemDataTable(ItemKey);
	if (!ensure(ItemInfo))
	{
		return;
	}

	ItemRank = ItemInfo->ItemRank;
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
