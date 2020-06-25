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

	if (PlayerExp)
	{
		FString expDisplayStr = FString::FormatAsNumber(Player->GetPlayerCurrentExp());
		expDisplayStr.Append(" / ");
		expDisplayStr.Append(FString::FromInt(Player->GetPlayerMaxExp()));
		PlayerExp->SetText(FText::FromString(expDisplayStr));
	}

	if (BattleClearCount)
	{
		BattleClearCount->SetText(FText::AsNumber(Player->GetBattleClearCount()));
	}

	if (GetCharacterCount)
	{
		GetCharacterCount->SetText(FText::AsNumber(Player->GetTotalCharacterCount()));
	}
}
