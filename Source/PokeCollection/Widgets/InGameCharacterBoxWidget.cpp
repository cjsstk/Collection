// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameCharacterBoxWidget.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"
#include "PokeCollectionHUD.h"
#include "PokeCharacter.h"

#include "Button.h"
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

	// Exclude joined party character
	TArray<APokeCharacter*> ShowCharacters = HaveCharacters;
	if (bIsMakingParty)
	{
		ShowCharacters.RemoveAll([](const APokeCharacter* PC) { return PC->GetJoinedPartyNum() > 0; });
	}

	const int32 SlotNum = CharacterGridPanel->GetChildrenCount();

	for (int32 Index = 0; Index < SlotNum; Index++)
	{
		UCharacterBoxSlot* CharacterSlot = Cast<UCharacterBoxSlot>(CharacterGridPanel->GetChildAt(Index));
		if (CharacterSlot)
		{
			if (ShowCharacters.IsValidIndex(Index))
			{
				APokeCharacter* CurrentCharacter = ShowCharacters[Index];
				if (!ensure(CurrentCharacter))
				{
					continue;
				}

				CharacterSlot->InitByID(CurrentCharacter->GetCharacterID());
				CharacterSlot->SetIsMakingParty(bIsMakingParty);
				CharacterSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if ((Index / ColumnNum) > ((ShowCharacters.Num() - 1) / ColumnNum))
				{
					CharacterSlot->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					CharacterSlot->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}

	}

}

void UCharacterBoxSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCharacterBoxSlot::OnSelectButtonClicked()
{
	Super::OnSelectButtonClicked();

	APokeCollectionHUD* PokeHud = GetOwningPlayer() ? Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD()) : nullptr;
	if (!ensure(PokeHud))
	{
		return;
	}

	if (bIsMakingParty)
	{
		APokeCollectionCharacter* Player = GetOwningPlayerPawn() ? Cast<APokeCollectionCharacter>(GetOwningPlayerPawn()) : nullptr;
		if (ensure(Player))
		{
			UInGameCharacterBoxWidget* CharacterBoxWidget = PokeHud->GetInGameCharacterBoxWidget();
			if (!CharacterBoxWidget)
			{
				return;
			}

			int32 SelectedSlotNum = CharacterBoxWidget->GetSelectedPartySlotNum();

			APokeCharacter* CurrentCharacter = Player->GetCharacterBySlotNum(1, SelectedSlotNum);
			if (CurrentCharacter)
			{
				CurrentCharacter->SetJoinedSlotNum(0);
				CurrentCharacter->SetJoinedPartyNum(0);
			}

			APokeCharacter* NextCharacter = Player->GetCharacterByID(ContentID);
			if (NextCharacter)
			{
				NextCharacter->SetJoinedSlotNum(SelectedSlotNum);
				NextCharacter->SetJoinedPartyNum(1);
			}

			PokeHud->OnBackButtonClicked(CharacterBoxWidget);
		}
	}
	else
	{
		PokeHud->OpenInGameCharacterInfoWidget(ContentID);
	}
}