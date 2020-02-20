// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameCharacterBoxWidget.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"
#include "PokeCharacter.h"

#include "Image.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"

void UInGameCharacterBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
			//GridSlot->SetHorizontalAlignment(HAlign_Fill);
			//GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

}

void UInGameCharacterBoxWidget::OnOpen()
{
	Super::OnOpen();

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

	CharacterGridPanel->ClearChildren();

	const TArray<APokeCharacter*>& HaveCharacters = Player->GetHaveCharacters();
	for (int32 Index = 0; Index < HaveCharacters.Num(); ++Index)
	{
		UCharacterBoxSlot* CharacterSlot = CreateWidget<UCharacterBoxSlot>(GetWorld(), CharacterBoxSlotClass.Get());
		if (!ensure(CharacterSlot))
		{
			continue;
		}

		characterKey CharacterKey = (HaveCharacters[Index])->GetCharacterKey();
		const FCharacterInfo* CharacterInfo = CMS::GetCharacterDataTable(CharacterKey);
		if (!CharacterInfo)
		{
			continue;
		}

		CharacterSlot->SetProfileImage(CharacterInfo->CharacterProfile);

		UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(CharacterSlot, Index / ColumnNum, Index % ColumnNum);
		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
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
