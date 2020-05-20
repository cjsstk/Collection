// Copyright Me. All Rights Reserved.


#include "ItemBoxWidget.h"

#include "UniformGridPanel.h"
#include "TextBlock.h"

#include "PokeCollectionCharacter.h"
#include "PokeItem.h"

void UItemBoxWidget::NativeConstruct()
{
	BoxContentType = EBoxContentType::Item;

	Super::NativeConstruct();
}

void UItemBoxWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	RefreshSlot();
}

void UItemBoxWidget::RefreshSlot()
{
	Super::RefreshSlot();

	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	if (!GridPanel)
	{
		return;
	}

	if (!BoxSlotClass.Get())
	{
		return;
	}

	const TArray<UPokeItem*>& HaveItems = Player->GetHaveItems();
	//

	TArray<ISortObjectInterface*> HaveSortItems;

	for (auto&& HaveItem : HaveItems)
	{
		HaveSortItems.Add(HaveItem);
	}

	HaveSortItems = SortObject(HaveSortItems);

	//
	const int32 SlotNum = GridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UItemSlot* BoxSlot = Cast<UItemSlot>(GridPanel->GetChildAt(Index));
		if (BoxSlot)
		{
			if (HaveSortItems.IsValidIndex(Index))
			{
				UPokeItem* CurrentItem = Cast<UPokeItem>(HaveSortItems[Index]);
				if (!ensure(CurrentItem))
				{
					continue;
				}

				BoxSlot->InitByID(CurrentItem->GetItemID());
				BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveSortItems.Num() - 1) / ColumnNum))
				{
					BoxSlot->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					BoxSlot->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}

	}
}

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	BoxContentType = EBoxContentType::Item;
}

void UItemSlot::InitByID(int32 InContentID)
{
	Super::InitByID(InContentID);

	APokeCollectionCharacter* Player = PokeCore::GetPokePlayer(GetWorld());
	if (!ensure(Player))
	{
		return;
	}

	UPokeItem* Item = Player->GetItemByID(InContentID);
	if (!Item)
	{
		return;
	}

	SetItemStack(Item->GetStackNum());
}

void UItemSlot::SetItemStack(int32 InStack)
{
	if (StackText)
	{
		StackText->SetText(FText::FromString(FString::FromInt(InStack)));
	}
}
