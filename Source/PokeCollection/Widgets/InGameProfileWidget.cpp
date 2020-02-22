// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameProfileWidget.h"

#include "PokeCollectionCharacter.h"

#include "TextBlock.h"

void UInGameProfileWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UInGameProfileWidget::OnOpen()
{
	Super::OnOpen();

	APokeCollectionCharacter* Player = GetPlayer();
	if (!ensure(Player))
	{
		return;
	}

	if (PlayerNickname)
	{
		PlayerNickname->SetText(FText::FromName(Player->GetPlayerNickName()));
	}

	if (PlayerLevel)
	{
		PlayerLevel->SetText(FText::FromString(FString::FormatAsNumber(Player->GetPlayerLevel())));
	}

	if (PlayerCurrentExp)
	{
		PlayerCurrentExp->SetText(FText::FromString(FString::FormatAsNumber(Player->GetPlayerCurrentExp())));
	}

	if (PlayerMaxExp)
	{
		PlayerMaxExp->SetText(FText::FromString(FString::FormatAsNumber(Player->GetPlayerMaxExp())));
	}
}
