// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBoxWidget.h"

#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"

#include "PokeCollectionCharacter.h"

void UCharacterBoxWidget::NativeConstruct() 
{
	Super::NativeConstruct();
}

void UCharacterBoxWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

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

	const TArray<APokeCharacter*>& HaveCharacters = Player->GetHaveCharacters();

	const int32 SlotNum = GridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UBoxSlot* BoxSlot = Cast<UBoxSlot>(GridPanel->GetChildAt(Index));
		if (BoxSlot)
		{
			if (HaveCharacters.IsValidIndex(Index))
			{
				APokeCharacter* CurrentCharacter = HaveCharacters[Index];
				if (!ensure(CurrentCharacter))
				{
					continue;
				}

				BoxSlot->SetContentImage(CurrentCharacter->GetCharacterProfileImage());
				BoxSlot->SetContentID(EBoxContentType::Character, CurrentCharacter->GetCharacterID());
				BoxSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveCharacters.Num() - 1) / ColumnNum))
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
