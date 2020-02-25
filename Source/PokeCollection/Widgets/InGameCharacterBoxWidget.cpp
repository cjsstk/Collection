// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameCharacterBoxWidget.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"
#include "PokeCharacter.h"

#include "Image.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "ScrollBox.h"

void UInGameCharacterBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	if (!CharacterGridPanel)
	{
		return;
	}

	CharacterGridPanel->ClearChildren();

	const int32 MaxHaveCharactersNum = Player->GetMaxHaveCharactersNum();
	for (int32 Index = 0; Index < MaxHaveCharactersNum; ++Index)
	{
		UCharacterBoxSlot* CharacterSlot = CreateWidget<UCharacterBoxSlot>(GetWorld(), CharacterBoxSlotClass.Get());
		if (!ensure(CharacterSlot))
		{
			return;
		}

		UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(CharacterSlot, Index / ColumnNum, Index % ColumnNum);
		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			//GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

}

void UInGameCharacterBoxWidget::OnOpen()
{
	Super::OnOpen();

	SetFocus();

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	if (!CharacterGridPanel)
	{
		return;
	}

	if (!CharacterBoxSlotClass.Get())
	{
		return;
	}
	
	const TArray<APokeCharacter*>& HaveCharacters = Player->GetHaveCharacters();
	const int32 SlotNum = CharacterGridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UCharacterBoxSlot* Slot = Cast<UCharacterBoxSlot>(CharacterGridPanel->GetChildAt(Index));
		if (Slot)
		{
			if (HaveCharacters.IsValidIndex(Index))
			{
				characterKey CharacterKey = (HaveCharacters[Index])->GetCharacterKey();
				const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
				if (!CharacterInfo)
				{
					continue;
				}

				Slot->SetProfileImage(CharacterInfo->CharacterProfile);
				Slot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if ((Index / ColumnNum) > ((HaveCharacters.Num() - 1) / ColumnNum))
				{
					Slot->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					Slot->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}

	}

}

void UCharacterBoxSlot::SetProfileImage(UTexture2D* InProfileTexture)
{
	if (!InProfileTexture)
	{
		return;
	}

	ProfileImage->SetBrushFromTexture(InProfileTexture);
}
