// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleStageInfoPopUp.h"

#include "Button.h"
#include "TextBlock.h"

#include "PokeCollectionHUD.h"

void UBattleStageSignboard::NativeConstruct()
{
	Super::NativeConstruct();

	const FBattleStageInfo* BattleStageInfo = CMS::GetBattleStageDataTable(BattleStageKey);
	if (BattleStageInfo)
	{
		if (BattleStageName)
		{
			BattleStageName->SetText(BattleStageInfo->BattleStageName);
		}
	}

	if (SignboardButton)
	{
		SignboardButton->OnClicked.AddUniqueDynamic(this, &UBattleStageSignboard::OnSignboardClicked);
	}
}

void UBattleStageSignboard::OnSignboardClicked()
{
	if (!GetOwningPlayer())
	{
		return;
	}

	APokeCollectionHUD* PokeHud = Cast<APokeCollectionHUD>(GetOwningPlayer()->GetHUD());
	if (PokeHud)
	{
		PokeHud->OpenBattleStageInfoPopUp();
	}
}
