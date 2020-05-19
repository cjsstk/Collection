// Copyright Me. All Rights Reserved.


#include "ItemBoxWidget.h"

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

	//APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	//if (!ensure(Player))
	//{
	//	return;
	//}

	//if (!GridPanel)
	//{
	//	return;
	//}

	//if (!BoxSlotClass.Get())
	//{
	//	return;
	//}

	//const TArray<APokeCharacter*>& HaveCharacters = Player->GetHaveCharacters();
	////

	//TArray<ISortObjectInterface*> HaveSortCharacters;

	//for (auto&& HaveCharacter : HaveCharacters)
	//{
	//	HaveSortCharacters.Add(HaveCharacter);
	//}

	//HaveSortCharacters = SortObject(HaveSortCharacters);

	////
	//const int32 SlotNum = GridPanel->GetChildrenCount();

	//for (int32 Index = 0; Index < SlotNum; Index++)
	//{
	//	UCharacterSlot* BoxSlot = Cast<UCharacterSlot>(GridPanel->GetChildAt(Index));
	//	if (BoxSlot)
	//	{
	//		if (HaveSortCharacters.IsValidIndex(Index))
	//		{
	//			APokeCharacter* CurrentCharacter = Cast<APokeCharacter>(HaveSortCharacters[Index]);
	//			if (!ensure(CurrentCharacter))
	//			{
	//				continue;
	//			}

	//			BoxSlot->InitByID(CurrentCharacter->GetCharacterID());
	//			BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//		}
	//		else
	//		{
	//			if ((Index / ColumnNum) > ((HaveSortCharacters.Num() - 1) / ColumnNum))
	//			{
	//				BoxSlot->SetVisibility(ESlateVisibility::Collapsed);
	//			}
	//			else
	//			{
	//				BoxSlot->SetVisibility(ESlateVisibility::Hidden);
	//			}
	//		}
	//	}

	//}
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
