// Copyright Me. All Rights Reserved.


#include "DestoryContentWidget.h"

#include "UniformGridPanel.h"
#include "Button.h"
#include "Image.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"

void UDestoryCharacterSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectedImage)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDestoryCharacterSlot::OnSelectButtonClicked()
{
	bSelected = !bSelected;

	if (bSelected)
	{
		if (SelectedImage)
		{
			SelectedImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (SelectedImage)
		{
			SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	OnDestroySlotClicked.Broadcast(bSelected, ContentID);
}

void UDestoryCharacterSlot::InitByKey(int32 InContentKey)
{
	Super::InitByKey(InContentKey);

	if (SelectedImage)
	{
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDestoryContentWidget::NativeConstruct()
{
	BoxContentType = EBoxContentType::Character;

	Super::NativeConstruct();

	if (DestroyButton)
	{
		DestroyButton->OnClicked.AddUniqueDynamic(this, &UDestoryContentWidget::OnDestroyButtonClicked);
	}
}

void UDestoryContentWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	RefreshSlot();
}

void UDestoryContentWidget::RefreshSlot()
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

	Player->SeletedDestroyCharacterIDs.Empty();

	const TArray<APokeCharacter*>& HaveCharacters = Player->GetHaveCharacters();
	//

	TArray<ISortObjectInterface*> HaveSortCharacters;

	for (auto&& HaveCharacter : HaveCharacters)
	{
		HaveSortCharacters.Add(HaveCharacter);
	}

	HaveSortCharacters = SortObject(HaveSortCharacters);

	//
	const int32 SlotNum = GridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UDestoryCharacterSlot* BoxSlot = Cast<UDestoryCharacterSlot>(GridPanel->GetChildAt(Index));
		if (BoxSlot)
		{
			if (HaveSortCharacters.IsValidIndex(Index))
			{
				APokeCharacter* CurrentCharacter = Cast<APokeCharacter>(HaveSortCharacters[Index]);
				if (!ensure(CurrentCharacter))
				{
					continue;
				}

				BoxSlot->InitByID(CurrentCharacter->GetCharacterID());
				BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				BoxSlot->OnDestroySlotClicked.AddUniqueDynamic(this, &UDestoryContentWidget::OnDestroySlotSelected);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveSortCharacters.Num() - 1) / ColumnNum))
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

void UDestoryContentWidget::OnDestroyButtonClicked()
{
	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (!PokeHud)
	{
		return;
	}

	PokeHud->OpenBasicPopUp(EBasicPopUpType::DestroyCharacter);
}

void UDestoryContentWidget::OnDestroySlotSelected(bool bSelected, int32 InCharacterID)
{
	APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
	if (!ensure(Player))
	{
		return;
	}

	if (bSelected)
	{
		Player->SeletedDestroyCharacterIDs.AddUnique(InCharacterID);
	}
	else
	{
		Player->SeletedDestroyCharacterIDs.Remove(InCharacterID);
	}
}
