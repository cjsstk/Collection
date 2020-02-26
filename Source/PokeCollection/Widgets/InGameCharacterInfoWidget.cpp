// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameCharacterInfoWidget.h"

#include "Image.h"
#include "TextBlock.h"

#include "PokeCharacter.h"
#include "PokeCollectionCharacter.h"

void UInGameCharacterInfoWidget::OnOpen()
{
	Super::OnOpen();

	APokeCollectionCharacter* Player = GetPlayer();
	if (ensure(Player))
	{
		APokeCharacter* SelectedCharacter = Player->GetCharacterByID(SelectedCharacterID);
		if (SelectedCharacter)
		{
			if (CharacterImage)
			{
				CharacterImage->SetBrushFromTexture(SelectedCharacter->GetCharacterImage());
			}
			if (CharacterName)
			{
				CharacterName->SetText(FText::FromName(SelectedCharacter->GetCharacterName()));
			}
		}
	}
}
