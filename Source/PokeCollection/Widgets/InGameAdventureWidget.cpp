// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameAdventureWidget.h"

#include "Image.h"

#include "CMS.h"
#include "PokeCollectionCharacter.h"

void UInGameAdventureWidget::OnOpen()
{
	Super::OnOpen();

	if (ensure(Background))
	{
		APokeCollectionCharacter* Player = Cast<APokeCollectionCharacter>(GetOwningPlayerPawn());
		if (ensure(Player))
		{
			const FStageInfo* StageInfo = CMS::GetStageDataTable(Player->GetCurrentSelectedStageNum());
			if (StageInfo)
			{
				Background->SetBrushFromTexture(StageInfo->StageBackground.LoadSynchronous());
			}
		}
	}
}

void UInGameAdventureWidget::OnBack()
{
	Super::OnBack();
}