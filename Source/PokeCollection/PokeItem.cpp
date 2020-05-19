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
}
